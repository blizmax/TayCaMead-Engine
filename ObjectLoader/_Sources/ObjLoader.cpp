#include "ObjLoader.h"

#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "ObjectLoader/MatLoader.h"

#include "TCMLogger/Logger.h"
#include "MathLib/Vec2.h"
#include "MathLib/Vec3.h"
#include "TCMRenderer/Rendering/RendererFactory.h"

namespace std
{
	template<>
	struct hash<TCM::Engine::Graphics::FragmentVertexIdx *>
	{
		size_t operator()( const TCM::Engine::Graphics::FragmentVertexIdx* k ) const
		{
			using std::size_t;
			using std::hash;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ( ( hash<uint16_t>()( k->m_idxVertex )
						^ ( hash<uint16_t>()( k->m_idxNormal ) << 1 ) ) >> 1 )
					^ ( hash<uint16_t>()( k->m_idxTexture ) << 1 );
		}
	};
}

// TODO: put in utils?
namespace TCM
{
	namespace Utils
	{
		std::string GetRawString( const std::string& baseStr )
		{
			std::string str;

			std::string::const_iterator it = baseStr.begin();
			std::string::const_iterator itEnd = baseStr.end();

			while ( it != itEnd )
			{
				char c = *it;

				switch ( c )
				{
					case '\n': str += "\\n";
						break;
					case '\t': str += "\\t";
						break;
					case '\r': str += "\\r";
						break;
					case '\b': str += "\\b";
						break;
					default: str += c;
						break;
				}
				++it;
			}

			return str;
		}

		std::vector<std::string> Split( const std::string& line, const char delim, const bool bRemoveEmpty = true )
		{
			size_t delimNum = count( line.begin(), line.end(), delim );

			std::vector<std::string> splitted;
			splitted.reserve( delimNum + 1 );

			std::stringstream sstream( line );
			std::string elem;

			while ( getline( sstream, elem, delim ) )
			{
				if ( elem.size() > 0 || !bRemoveEmpty )
					splitted.push_back( elem );
			}

			return splitted;
		}

		template<typename T>
		void DeleteVectorData( std::vector<T *>& vec )
		{
			for ( T* oneItem : vec )
			{
				if ( oneItem != nullptr )
					delete oneItem;
			}

			vec.clear();
		}
	}

	namespace Engine
	{
		namespace Resources
		{
			TCMObjLoader::TCMObjLoader()
			{
				ClearTemporaryData();
			}

			void TCMObjLoader::ClearTemporaryData()
			{
				m_vecVertex.clear();
				m_vecVertex.push_back( Math::Vec3() );

				m_vecNormal.clear();
				m_vecNormal.push_back( Math::Vec3() );

				m_vecTexture.clear();
				m_vecTexture.push_back( Math::Vec2() );

				Utils::DeleteVectorData( m_vecFragmentVertex );
				Utils::DeleteVectorData( m_vecFragment );
				Utils::DeleteVectorData( m_vertexDataList );

				m_mapFragVertexIndex.clear();

				m_offset = 0;
				m_objDirectory = "";

				m_lowerLeftFront = Math::Vec3( 999999.0f, 999999.0f, -999999.0f );
				m_upperRightBack = Math::Vec3( -999999.0f, -999999.0f, 999999.0f );
			}

			TCMObjLoader& TCMObjLoader::GetInstance()
			{
				static std::unique_ptr<TCMObjLoader> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new TCMObjLoader() );
				                }
				);
				return *( m_instance.get() );
			}

			Graphics::s_Skeleton* TCMObjLoader::LoadFromFile( const std::string& directory, const std::string& fileName )
			{
				GetInstance().SetTemporaryObjDirectory( directory );
				return LoadFromFile( directory + fileName );
			}

			Graphics::s_Skeleton* TCMObjLoader::LoadFromFile( const std::string& path )
			{
				if ( path.size() <= 0 )
				{
					TCMWARNING("Path given is a bad value");
					GetInstance().SetTemporaryObjDirectory( "" );
					return nullptr;
				}
				Graphics::s_Skeleton* result = nullptr;

				try
				{
					char* buffer;

					// Put file content in buffer
					{
						std::ifstream fileStream( path, std::ios::binary );
						if ( !fileStream.is_open() )
						{
							throw std::runtime_error( "TCMObjLoader::ParseObjFile() - could not open file [" + std::string( path ) + "]" );
						}
						auto begin = fileStream.tellg();
						fileStream.seekg( 0, std::ios::end );
						auto file_len = fileStream.tellg() - begin;
						buffer = new char[file_len + 1];
						fileStream.seekg( 0, std::ios::beg );
						fileStream.read( buffer, file_len );
						fileStream.close();
					}

					std::string fileContent( buffer );
					delete[] buffer;

					GetInstance().ParseObjFile( fileContent, result );
				}
				catch ( std::runtime_error e )
				{
					TCMFAILURE("TCMObjLoader has thrown an exception: " + std::string(e.what()));
				}

				GetInstance().SetTemporaryObjDirectory( "" );

				return result;
			}

#include <chrono>

			void TCMObjLoader::ParseObjFile( std::string& fileContent, Graphics::Skeleton*& skeleton )
			{
				auto begin = std::chrono::high_resolution_clock::now();

				std::string oneLine( "" );

				// Clean file content - Removes '\r', replace '\t' with ' ', and remove doubled or more ' '
				{
					fileContent.erase( remove( fileContent.begin(), fileContent.end(), '\r' ), fileContent.end() );
					replace( fileContent.begin(), fileContent.end(), '\t', ' ' );
					std::string::iterator newEnd =
							std::unique( fileContent.begin(), fileContent.end(),
							             [=](char lhs, char rhs)
							             {
								             return ( lhs == rhs ) && ( lhs == ' ' );
							             }
							);
					fileContent.erase( newEnd, fileContent.end() );
				}

				std::istringstream formatedContent( fileContent );

				std::vector<Graphics::AbstractMaterial *> vecListMaterials;
				std::vector<Graphics::SubMesh*> subMeshes;

				Graphics::SubMesh* bone = new Graphics::SubMesh;

				while ( getline( formatedContent, oneLine ) )
				{
					std::vector<std::string> splittedLine = Utils::Split( oneLine, ' ' );
					ELineType lineType = GetLineType( splittedLine );

					switch ( lineType )
					{
						case V:
						{
							ParseVertex( splittedLine );
							break;
						}

						case VT:
						{
							ParseTexture( splittedLine );
							break;
						}

						case VN:
						{
							ParseNormal( splittedLine );
							break;
						}

						case F:
						{
							ParseFragment( splittedLine );
							break;
						}

						case O:
						{
							ParseObject( splittedLine, bone );
							subMeshes.push_back( bone );
							break;
						}

						case G:
						{
							ParseObject( splittedLine, bone );
							subMeshes.push_back( bone );
							break;
						}

						case MTLLIB:
						{
							ParseMtlLib( splittedLine, vecListMaterials );
							break;
						}

						case USEMTL:
						{
							ParseUseMtl( splittedLine, bone, vecListMaterials );
							break;
						}

						case BADTYPE:
						{
							/*
							fileStream.close();
							throw std::runtime_error("TCMObjLoader::ParseObjFile() - ERROR when parsing line [" + oneLine + "]");
							*/
							TCMWARNING("Got a ELineType::BADTYPE ! [" + Utils::GetRawString(oneLine) + "]");
						}

						default:
							break;
					}
				}

				CreateBone( bone, "" );

				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( m_vertexDataList, m_vecFragment );
				if ( mesh != nullptr )
					skeleton = new Graphics::Skeleton;

				Math::Vec3 boxSize;
				Math::Vec3 offset = ( m_upperRightBack + m_lowerLeftFront ) / 2;
				boxSize.z = abs( m_upperRightBack.z - m_lowerLeftFront.z );
				boxSize.x = abs( m_upperRightBack.x - m_lowerLeftFront.x );
				boxSize.y = abs( m_upperRightBack.y - m_lowerLeftFront.y );
				Math::Bounds::Box box = Math::Bounds::Box::FromVec3( boxSize );
				box.position = offset;

				mesh->SetBox( box );

				skeleton->SetMesh( mesh );

				for ( int i = 0; i < subMeshes.size(); ++i )
				{
					Graphics::s_SubMesh* subMesh = subMeshes[i];
					if ( subMesh->GetCount() > 0 )
						skeleton->AddSubMesh( subMesh );
					else
						delete subMesh;
				}

				ClearTemporaryData();

				auto end = std::chrono::high_resolution_clock::now();

				std::chrono::duration<float> len = end - begin;
				TCMDEBUG("DURATION === [" + std::to_string(len.count()) + "]");
			}

			TCMObjLoader::ELineType TCMObjLoader::GetLineType( const std::vector<std::string>& splitted ) const
			{
				if ( splitted.size() < 1 || splitted[0][0] == '#' )
					return EMPTY;
				size_t strHash = std::hash<std::string>()( splitted[0] );

				if ( strHash == std::hash<std::string>()( "v" ) ) return V;
				else if ( strHash == std::hash<std::string>()( "vt" ) ) return VT;
				else if ( strHash == std::hash<std::string>()( "vn" ) ) return VN;
				else if ( strHash == std::hash<std::string>()( "f" ) ) return F;
				else if ( strHash == std::hash<std::string>()( "o" ) ) return O;
				else if ( strHash == std::hash<std::string>()( "g" ) ) return G;
				else if ( strHash == std::hash<std::string>()( "mtllib" ) ) return MTLLIB;
				else if ( strHash == std::hash<std::string>()( "usemtl" ) ) return USEMTL;

				return BADTYPE;
			}

			void TCMObjLoader::ParseVertex( const std::vector<std::string>& splitted )
			{
				if ( splitted.size() != 4 ) // v x y z
					throw std::runtime_error( "TCMObjLoader::ParseVertex() - bad number of values" );

				Math::Vec3 vertex = Math::Vec3( stof( splitted[1] ),
				                                stof( splitted[2] ),
				                                stof( splitted[3] ) );

				m_lowerLeftFront.x = ( m_lowerLeftFront.x < vertex.x ) ?
					                     m_lowerLeftFront.x : vertex.x;
				m_lowerLeftFront.y = ( m_lowerLeftFront.y < vertex.y ) ?
					                     m_lowerLeftFront.y : vertex.y;
				m_lowerLeftFront.z = ( m_lowerLeftFront.z > vertex.z ) ?
					                     m_lowerLeftFront.z : vertex.z;

				// Update upper-right-back corner of BB
				m_upperRightBack.x = ( m_upperRightBack.x > vertex.x ) ?
					                     m_upperRightBack.x : vertex.x;
				m_upperRightBack.y = ( m_upperRightBack.y > vertex.y ) ?
					                     m_upperRightBack.y : vertex.y;
				m_upperRightBack.z = ( m_upperRightBack.z < vertex.z ) ?
					                     m_upperRightBack.z : vertex.z;

				m_vecVertex.push_back( vertex );
			}

			void TCMObjLoader::ParseNormal( const std::vector<std::string>& splitted )
			{
				if ( splitted.size() != 4 ) // vn xn yn zn
					throw std::runtime_error( "TCMObjLoader::ParseNormal() - bad number of values" );

				m_vecNormal.push_back(
					Math::Vec3( stof( splitted[1] ),
					            stof( splitted[2] ),
					            stof( splitted[3] ) )
				);
			}

			void TCMObjLoader::ParseTexture( const std::vector<std::string>& splitted )
			{
				if ( splitted.size() != 3 && splitted.size() != 4 ) // vt u v (0)
					throw std::runtime_error( "TCMObjLoader::ParseTexture() - bad number of values" );

				m_vecTexture.push_back(
					Math::Vec2( stof( splitted[1] ),
					            stof( splitted[2] ) )
				);
			}

			void TCMObjLoader::ParseFragment( const std::vector<std::string>& splitted )
			{
				if ( splitted.size() == 4 ) // f v1/t1/n1 v2/t2/n2 v3/t3/n3
				{
					uint16_t indexes[3];

					indexes[0] = ParseFragmentVertex( splitted, 1 );
					indexes[1] = ParseFragmentVertex( splitted, 2 );
					indexes[2] = ParseFragmentVertex( splitted, 3 );

					CreateFragment( indexes[0], indexes[1], indexes[2] );
				}
				else if ( splitted.size() == 5 )
				{
					uint16_t indexes[4];

					indexes[0] = ParseFragmentVertex( splitted, 1 );
					indexes[1] = ParseFragmentVertex( splitted, 2 );
					indexes[2] = ParseFragmentVertex( splitted, 3 );
					indexes[3] = ParseFragmentVertex( splitted, 4 );

					CreateQuad( indexes[0], indexes[1], indexes[2], indexes[3] );
				}
				else
					throw std::runtime_error( "TCMObjLoader::ParseFragment() - bad number of values" );
			}

			void TCMObjLoader::CreateQuad( const uint16_t first, const uint16_t second, const uint16_t third, const uint16_t fourth )
			{
				CreateFragment( first, second, fourth );
				CreateFragment( fourth, second, third );
			}

			void TCMObjLoader::CreateFragment( const uint16_t first, const uint16_t second, const uint16_t third )
			{
				Graphics::Fragment* newFragment = new Graphics::Fragment();

				newFragment->m_idxFragmentVertex[0] = first;
				newFragment->m_idxFragmentVertex[1] = second;
				newFragment->m_idxFragmentVertex[2] = third;

				m_vecFragment.push_back( newFragment );
			}

			uint16_t TCMObjLoader::ParseFragmentVertex( const std::vector<std::string>& splitted, const uint8_t vertIdx )
			{
				std::vector<std::string> oneFragmentVertex = Utils::Split( splitted[vertIdx], '/' );

				int32_t tmp;

				uint16_t idxVertex;
				uint16_t idxTexture = 0;
				uint16_t idxNormal = 0;

				if ( oneFragmentVertex.size() == 3 ) // v t n
				{
					tmp = stoi( oneFragmentVertex[0] );
					idxVertex = ( tmp < 0 ) ? static_cast<uint16_t>(m_vecVertex.size() + tmp) : tmp;

					tmp = stoi( oneFragmentVertex[1] );
					idxTexture = ( tmp < 0 ) ? static_cast<uint16_t>(m_vecTexture.size() + tmp) : tmp;

					tmp = stoi( oneFragmentVertex[2] );
					idxNormal = ( tmp < 0 ) ? static_cast<uint16_t>(m_vecNormal.size() + tmp) : tmp;
				}
				else if ( oneFragmentVertex.size() == 2 ) // v t
				{
					tmp = stoi( oneFragmentVertex[0] );
					idxVertex = ( tmp < 0 ) ? static_cast<uint16_t>(m_vecVertex.size() + tmp) : tmp;

					tmp = stoi( oneFragmentVertex[1] );
					idxTexture = ( tmp < 0 ) ? static_cast<uint16_t>(m_vecTexture.size() + tmp) : tmp;
				}
				else if ( oneFragmentVertex.size() == 1 ) // v
				{
					tmp = stoi( oneFragmentVertex[0] );
					idxVertex = ( tmp < 0 ) ? static_cast<uint16_t>(m_vecVertex.size() + tmp) : tmp;
				}
				else
					throw std::runtime_error( "TCMObjLoader::ParseFragmentVertex() - bad fragment vertex format [" + splitted[vertIdx] + "]" );

				return CreateFragmentVertex( idxVertex, idxTexture, idxNormal );
			}

			uint16_t TCMObjLoader::CreateFragmentVertex( const uint16_t& idxVertex, const uint16_t& idxTexture, const uint16_t& idxNormal )
			{
				Graphics::FragmentVertexIdx* newFragmentVertex = new Graphics::FragmentVertexIdx();

				newFragmentVertex->m_idxVertex = idxVertex;
				newFragmentVertex->m_idxNormal = idxNormal;
				newFragmentVertex->m_idxTexture = idxTexture;

				//Hash only once
				size_t key = std::hash<const Graphics::FragmentVertexIdx *>()( newFragmentVertex );

				auto search = m_mapFragVertexIndex.find( key );

				uint16_t index;

				if ( search == m_mapFragVertexIndex.end() )
				{
					Graphics::VertexData* objectFragmentVertex = new Graphics::VertexData();

					// Set values in FragmentVertex using IDX - FRAGMENT VERTEX = VERT / NORM / TEX
					objectFragmentVertex->m_vertex = m_vecVertex[newFragmentVertex->m_idxVertex];
					objectFragmentVertex->m_normal = m_vecNormal[newFragmentVertex->m_idxNormal];
					objectFragmentVertex->m_texture = m_vecTexture[newFragmentVertex->m_idxTexture];

					m_vertexDataList.push_back( objectFragmentVertex );
					m_vecFragmentVertex.push_back( newFragmentVertex );

					index = static_cast<int>(m_vertexDataList.size() - 1);
					m_mapFragVertexIndex.insert( std::make_pair( key, index ) );
				}
				else
				{
					delete newFragmentVertex;
					index = search->second;
				}

				return index;
			}

			void TCMObjLoader::ParseObject( const std::vector<std::string>& splitted, Graphics::SubMesh*& currentBone )
			{
				CreateBone( currentBone, splitted[1] );
			}

			void TCMObjLoader::ParseGroup( const std::vector<std::string>& splitted, Graphics::SubMesh*& currentBone )
			{
				if ( m_vecFragment.size() == 0 )
					return;

				CreateBone( currentBone, "" );

				/*if ( splitted.size() > 1 )
				{
					currentBone->m_name( m_objDirectory + " " + currentSceneObject->GetName() + " " + splitted[1] );
				}
				else
				{
					currentBone->m_name( m_objDirectory + " " + currentSceneObject->GetName() );
				}

				currentSceneObject->AddComponent( currentBone );*/
			}

			void TCMObjLoader::ParseMtlLib( const std::vector<std::string>& splittedLine, std::vector<Graphics::AbstractMaterial *>& vecListMaterials ) const
			{
				size_t length = splittedLine.size();
				if ( length < 2 ) // mtllib path
					throw std::runtime_error( "TCMObjLoader::ParseMtlLib() - bad line size [" + std::to_string( splittedLine.size() ) + "]" );

				std::string fullName( "" );
				for ( size_t i = 1; i < length - 1; i++ )
				{
					fullName.append( splittedLine[i] + " " );
				}
				fullName.append( splittedLine[length - 1] );

				vecListMaterials = MatLoader::LoadFromFile( m_objDirectory, fullName );
			}

			void TCMObjLoader::ParseUseMtl( const std::vector<std::string>& splittedLine, Graphics::SubMesh* currentBone, const std::vector<Graphics::AbstractMaterial*>& vecListMaterials ) const
			{
				size_t length = splittedLine.size();
				if ( length < 2 ) // usemtl path
					throw std::runtime_error( "TCMObjLoader::ParseUseMtl() - bad line size" );

				std::string fullName( "" );
				for ( size_t i = 1; i < length - 1; i++ )
				{
					fullName.append( splittedLine[i] + " " );
				}
				fullName.append( splittedLine[length - 1] );

				for ( Graphics::AbstractMaterial* oneMaterial : vecListMaterials )
				{
					if ( oneMaterial->GetName().compare( fullName ) == 0 )
					{
						currentBone->SetMaterial( oneMaterial );
						return;
					}
				}
			}

			void TCMObjLoader::SetTemporaryObjDirectory( const std::string& directory )
			{
				m_objDirectory = directory;
			}

			void TCMObjLoader::ComputeTangents( std::vector<Graphics::VertexData*>* vertexDataList, std::vector<Graphics::Fragment *>* fragmentList ) const
			{
				Graphics::VertexData** vertexDataArray = vertexDataList->data();
				Graphics::Fragment** fragmentArray = fragmentList->data();

				size_t vertexDataCount = vertexDataList->size();
				size_t fragmentDataCount = fragmentList->size();

				for ( int i = 0; i < fragmentDataCount; ++i )
				{
					Graphics::Fragment* objectFragment = fragmentArray[i];
					// Shortcuts for vertices
					Math::Vec3& v0 = vertexDataArray[objectFragment->m_idxFragmentVertex[0]]->m_vertex;
					Math::Vec3& v1 = vertexDataArray[objectFragment->m_idxFragmentVertex[1]]->m_vertex;
					Math::Vec3& v2 = vertexDataArray[objectFragment->m_idxFragmentVertex[2]]->m_vertex;

					// Shortcuts for UVs
					Math::Vec2& uv0 = vertexDataArray[objectFragment->m_idxFragmentVertex[0]]->m_texture;
					Math::Vec2& uv1 = vertexDataArray[objectFragment->m_idxFragmentVertex[1]]->m_texture;
					Math::Vec2& uv2 = vertexDataArray[objectFragment->m_idxFragmentVertex[2]]->m_texture;

					// Edges of the triangle : postion delta
					Math::Vec3 deltaPos1 = v1 - v0;
					Math::Vec3 deltaPos2 = v2 - v0;

					// UV delta
					Math::Vec2 deltaUV1 = uv1 - uv0;
					Math::Vec2 deltaUV2 = uv2 - uv0;

					float r = 1.0f / ( deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x );
					Math::Vec3 tangent = ( deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y ) * r;

					vertexDataArray[objectFragment->m_idxFragmentVertex[0]]->m_tangent += tangent;
					vertexDataArray[objectFragment->m_idxFragmentVertex[1]]->m_tangent += tangent;
					vertexDataArray[objectFragment->m_idxFragmentVertex[2]]->m_tangent += tangent;
				}

				for ( int i = 0; i < vertexDataCount; ++i )
				{
					vertexDataArray[i]->m_tangent.Normalize();
				}
			}

			void TCMObjLoader::CreateBone( Graphics::SubMesh*& bone, const std::string& name )
			{
				if ( bone != nullptr )
				{
					uint32_t offset = static_cast<uint32_t>(m_offset * 3);
					m_offset = m_vecFragment.size();
					uint32_t indicesSize = static_cast<uint32_t>(m_offset * 3 - offset);

					bone->SetParams( offset, indicesSize );
					bone->SetName( name );
					bone->SetBox( GetBoundingBox( offset, indicesSize ) );
				}
				bone = new Graphics::SubMesh;
			}

			Math::Bounds::Box TCMObjLoader::GetBoundingBox( const size_t begin, const size_t count )
			{
				Math::Vec3 vLowerLeftFront = Math::Vec3( 999999.0f, 999999.0f, -999999.0f );
				Math::Vec3 vUpperRightBack = Math::Vec3( -999999.0f, -999999.0f, 999999.0f );

				for ( size_t i = 0; i < count; ++i )
				{
					Graphics::Fragment* objectFragment = m_vecFragment[( i + begin ) / 3];
					Math::Vec3 v = m_vertexDataList[objectFragment->m_idxFragmentVertex[( i + begin ) % 3]]->m_vertex;

					vLowerLeftFront.x = ( vLowerLeftFront.x < v.x ) ?
						                    vLowerLeftFront.x : v.x;
					vLowerLeftFront.y = ( vLowerLeftFront.y < v.y ) ?
						                    vLowerLeftFront.y : v.y;
					vLowerLeftFront.z = ( vLowerLeftFront.z > v.z ) ?
						                    vLowerLeftFront.z : v.z;

					// Update upper-right-back corner of BB
					vUpperRightBack.x = ( vUpperRightBack.x > v.x ) ?
						                    vUpperRightBack.x : v.x;
					vUpperRightBack.y = ( vUpperRightBack.y > v.y ) ?
						                    vUpperRightBack.y : v.y;
					vUpperRightBack.z = ( vUpperRightBack.z < v.z ) ?
						                    vUpperRightBack.z : v.z;
				}

				Math::Vec3 boxSize;
				Math::Vec3 offset = ( vUpperRightBack + vLowerLeftFront ) / 2;
				boxSize.z = abs( vUpperRightBack.z - vLowerLeftFront.z );
				boxSize.x = abs( vUpperRightBack.x - vLowerLeftFront.x );
				boxSize.y = abs( vUpperRightBack.y - vLowerLeftFront.y );
				Math::Bounds::Box box = Math::Bounds::Box::FromVec3( boxSize );
				box.position = offset;

				return box;
			}
		}
	}
}
