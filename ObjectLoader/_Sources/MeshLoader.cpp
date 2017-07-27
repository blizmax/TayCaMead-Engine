#include <algorithm>
#include <memory>
#include <mutex>
#include <sstream>

#include "ObjectLoader/MeshLoader.h"
#include "FbxLoader.h"
#include "ObjLoader.h"

#include "TCMLogger/Logger.h"
#include "MathLib/Quat.h"

namespace TCM
{
	namespace Engine
	{
		namespace Resources
		{
			MeshLoader& MeshLoader::GetInstance()
			{
				static std::unique_ptr<MeshLoader> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new MeshLoader() );
				                }
				);
				return *( m_instance.get() );
			}

			Graphics::Skeleton* MeshLoader::CoreLoadMesh( const std::string& filepath ) const
			{
				FileType type = GetFileType( filepath );
				if ( type == FBX )
					return FbxLoader::LoadFromFile( filepath );
				else if ( type == OBJ )
				{
					size_t lastOf = filepath.find_last_of( "\\/" );
					std::string directory = filepath.substr( 0, lastOf );
					std::string filename = filepath.substr( lastOf + 1 );
					return TCMObjLoader::LoadFromFile( directory, filename );
				}
				return nullptr;
			}

			Graphics::Skeleton* MeshLoader::CoreLoadMesh( const std::string& directory, const std::string& filename ) const
			{
				FileType type = GetFileType( filename );

				if ( type == FBX )
					return FbxLoader::LoadFromFile( directory + filename );
				else if ( type == OBJ )
					return TCMObjLoader::LoadFromFile( directory, filename );
				return nullptr;
			}

			MeshLoader::FileType MeshLoader::GetFileType( const std::string& filename ) const
			{
				std::string fn = filename.substr( filename.find_last_of( "." ) + 1 );
				transform( fn.begin(), fn.end(), fn.begin(), tolower );

				size_t hashCode = std::hash<std::string>()( fn );
				if ( hashCode == std::hash<std::string>()( "fbx" ) )
					return FBX;
				else if ( hashCode == std::hash<std::string>()( "obj" ) )
					return OBJ;

				TCMFAILURE("File extension not supported.");
				return NONE;
			}

			Graphics::Skeleton* MeshLoader::LoadMesh( const std::string& filepath )
			{
				return GetInstance().CoreLoadMesh( filepath );
			}

			Graphics::Skeleton* MeshLoader::LoadMesh( const std::string& directory, const std::string& filename )
			{
				return GetInstance().CoreLoadMesh( directory, filename );
			}

			void MeshLoader::ComputeTangents( std::vector<Graphics::VertexData*>* vertexDataList, std::vector<Graphics::Fragment *>* fragmentList ) const
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
		}
	}
}
