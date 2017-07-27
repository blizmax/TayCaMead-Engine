#include "TCMResourceManager/TCMResourceManager.h"

#include "TCMLogger/Logger.h"
#include "TCMRenderer/Rendering/RendererFactory.h"

#include <memory>
#include <mutex>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "ObjectLoader/MeshLoader.h"
#include <sstream>
#include "ObjectLoader/PrimitiveLoader.h"

namespace TCM
{
	namespace Utils
	{
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
	}

	namespace Engine
	{
		namespace Resources
		{
#pragma region Constructor and co.

			ResourceManager::~ResourceManager()
			{
				{
					std::unordered_map<std::string, Graphics::AbstractMesh *>::iterator end = m_hashmapMesh.end();
					for ( std::unordered_map<std::string, Graphics::AbstractMesh *>::iterator iter = m_hashmapMesh.begin(); iter != end; ++iter )
					{
						delete iter->second;
					}
					m_hashmapMesh.clear();
				}

				{
					std::unordered_map<std::string, Graphics::Skeleton *>::iterator end = m_hashmapSkeleton.end();
					for ( std::unordered_map<std::string, Graphics::Skeleton *>::iterator iter = m_hashmapSkeleton.begin(); iter != end; ++iter )
					{
						delete iter->second;
					}
					m_hashmapSkeleton.clear();
				}
				{
					std::unordered_map<std::string, Graphics::AbstractShader *>::iterator end = m_hashmapShader.end();
					for ( std::unordered_map<std::string, Graphics::AbstractShader *>::iterator iter = m_hashmapShader.begin(); iter != end; ++iter )
					{
						delete iter->second;
					}
					m_hashmapShader.clear();
				}
				{
					std::unordered_map<std::string, Graphics::AbstractTexture *>::iterator end = m_hashmapTexture.end();
					for ( std::unordered_map<std::string, Graphics::AbstractTexture *>::iterator iter = m_hashmapTexture.begin(); iter != end; ++iter )
					{
						delete iter->second;
					}
					m_hashmapTexture.clear();
				}
				{
					std::unordered_map<std::string, Graphics::AbstractMaterial *>::iterator end = m_hashmapMaterial.end();
					for ( std::unordered_map<std::string, Graphics::AbstractMaterial *>::iterator iter = m_hashmapMaterial.begin(); iter != end; ++iter )
					{
						delete iter->second;
					}
					m_hashmapMaterial.clear();
				}
			}

			bool ResourceManager::CoreRegisterMesh( Graphics::AbstractMesh* mesh )
			{
				if ( mesh == nullptr )
					return false;
				std::string key = mesh->GetKey();

				auto iter = m_hashmapMesh.find( key );
				if ( iter == m_hashmapMesh.end() )
				{
					m_hashmapMesh[key] = mesh;
					return true;
				}

				return false;
			}

			ResourceManager::PrimitiveForm ResourceManager::GetForm( const std::string& key ) const
			{
				size_t hashCode = std::hash<std::string>()( key );

				if ( hashCode == std::hash<std::string>()( "Sphere" ) ) return PrimitiveForm::SPHERE;
				else if ( hashCode == std::hash<std::string>()( "Tube" ) ) return PrimitiveForm::TUBE;
				else if ( hashCode == std::hash<std::string>()( "Cone" ) ) return PrimitiveForm::CONE;
				else if ( hashCode == std::hash<std::string>()( "Torus" ) ) return PrimitiveForm::TORUS;
				else if ( hashCode == std::hash<std::string>()( "Capsule" ) ) return PrimitiveForm::CAPSULE;
				else if ( hashCode == std::hash<std::string>()( "Plane" ) ) return PrimitiveForm::PLANE;
				else if ( hashCode == std::hash<std::string>()( "Box" ) ) return PrimitiveForm::BOX;

				return PrimitiveForm::COUNT;
			}

			Graphics::AbstractMesh* ResourceManager::GenPrimitive( const std::string& key ) const
			{
				std::vector<std::string> splitted = Utils::Split( key, ' ' );
				if ( splitted.size() <= 1 )
					return nullptr;

				PrimitiveForm primitiveForm = GetForm( splitted[0] );
				if ( primitiveForm == PrimitiveForm::COUNT )
					return nullptr;

				Graphics::AbstractMesh* result = nullptr;

				switch ( primitiveForm )
				{
					case PrimitiveForm::SPHERE:
					{
						float radius = std::stof( splitted[1] );
						int nbLong = std::stoi( splitted[2] );
						int nbLag = std::stoi( splitted[3] );;
						result = TCMPrimitiveLoader::GenSphere( radius, nbLong, nbLag );
						break;
					}
					case PrimitiveForm::TUBE:
					{
						float height = std::stof( splitted[1] );
						int nbSides = std::stoi( splitted[2] );
						float bottomRadius1 = std::stof( splitted[3] );
						float bottomRadius2 = std::stof( splitted[4] );
						float topRadius1 = std::stof( splitted[5] );
						float topRadius2 = std::stof( splitted[6] );
						result = TCMPrimitiveLoader::GenTube( height, nbSides, bottomRadius1, bottomRadius2,
						                                      topRadius1, topRadius2 );
						break;
					}
					case PrimitiveForm::CONE:
					{
						float height = std::stof( splitted[1] );
						int nbSides = std::stoi( splitted[2] );
						float bottomRadius = std::stof( splitted[3] );
						float topRadius = std::stof( splitted[4] );
						result = TCMPrimitiveLoader::GenCone( height, nbSides, bottomRadius, topRadius );
						break;
					}
					case PrimitiveForm::TORUS:
					{
						float radius1 = std::stof( splitted[1] );
						float radius2 = std::stof( splitted[2] );
						int nbRadSeg = std::stoi( splitted[3] );
						int nbSides = std::stoi( splitted[4] );
						result = TCMPrimitiveLoader::GenTorus( radius1, radius2, nbRadSeg, nbSides );
						break;
					}
					case PrimitiveForm::CAPSULE:
					{
						float height = std::stof( splitted[1] );
						float radius = std::stof( splitted[2] );
						int nbSides = std::stoi( splitted[3] );
						result = TCMPrimitiveLoader::GenCapsule( height, radius, nbSides );
						break;
					}
					case PrimitiveForm::PLANE:
					{
						float length = std::stof( splitted[1] );
						float width = std::stof( splitted[2] );
						int resX = std::stoi( splitted[3] );
						int resZ = std::stoi( splitted[4] );
						result = TCMPrimitiveLoader::GenPlane( length, width, resX, resZ );
						break;
					}
					case PrimitiveForm::BOX:
					{
						float length = std::stof( splitted[1] );
						float width = std::stof( splitted[2] );
						float height = std::stof( splitted[3] );
						result = TCMPrimitiveLoader::GenBox( length, width, height );
						break;
					}
					default: result = nullptr;
				}
				RegisterMesh( result );
				return result;
			}

			Graphics::AbstractMesh* ResourceManager::GetMesh( const std::string& meshKey )
			{
				return GetInstance().CoreGetMesh( meshKey );
			}

			Graphics::AbstractMesh* ResourceManager::GetMesh( const std::string& directory, const std::string& fileName )
			{
				return GetInstance().CoreGetMesh( directory, fileName );
			}

			bool ResourceManager::RegisterMesh( Graphics::AbstractMesh* mesh )
			{
				return GetInstance().CoreRegisterMesh( mesh );
			}

			Graphics::Skeleton* ResourceManager::GetSkeleton( const std::string& meshKey )
			{
				return GetInstance().CoreGetSkeleton( meshKey );
			}

			Graphics::Skeleton* ResourceManager::GetSkeleton( const std::string& directory, const std::string& fileName )
			{
				return GetInstance().CoreGetSkeleton( directory, fileName );
			}

			Graphics::AbstractShader* ResourceManager::GetShader( const std::string& vertexShaderPath, const std::string& fragmentShaderPath )
			{
				return GetInstance().CoreGetShader( vertexShaderPath, fragmentShaderPath );
			}

			Graphics::AbstractMaterial* ResourceManager::GetMaterial( const std::string& materialKey )
			{
				return GetInstance().CoreGetMaterial( materialKey );
			}

			Graphics::AbstractTexture* ResourceManager::GetTexture( const std::string& textureKey )
			{
				return GetInstance().CoreGetTexture( textureKey );
			}

			ResourceManager& ResourceManager::GetInstance()
			{
				static std::unique_ptr<ResourceManager> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new ResourceManager() );
					                m_instance->Init();
				                }
				);
				return *( m_instance.get() );
			}

			void ResourceManager::Init()
			{
				Graphics::AbstractMaterial* mat = Graphics::RendererFactory::CreateMaterial();
				m_hashmapMaterial[""] = mat;

				Graphics::AbstractShader* shader = Graphics::RendererFactory::CreateShader(
					"../Common/TCMRenderer/OpenGLShaders/base.vert"
					, "../Common/TCMRenderer/OpenGLShaders/complex.frag" );

				m_hashmapShader[""] = shader;
			}
#pragma endregion

#pragma region Mesh

			Graphics::Skeleton* ResourceManager::CoreCreateSkeleton( const std::string& directory, const std::string& fileName )
			{
				std::string key = directory + fileName;
				Graphics::Skeleton* skeleton = MeshLoader::LoadMesh( directory, fileName );
				if ( skeleton == nullptr )
					return nullptr;
				m_hashmapSkeleton[key] = skeleton;
				Graphics::AbstractMesh* mesh = skeleton->GetMesh();
				mesh->SetKey( key );
				CoreRegisterMesh( mesh );

				for ( int i = 0; i < skeleton->GetSubMeshCount(); ++i )
				{
					Graphics::SubMesh* bone = skeleton->GetSubMesh( i );
					bone->SetMesh( mesh );
					CoreRegisterMaterial( bone->GetMaterial() );
				}
				return skeleton;
			}

			Graphics::AbstractMesh* ResourceManager::CoreGetMesh( const std::string& meshKey )
			{
				auto iter = m_hashmapMesh.find( meshKey );
				if ( iter == m_hashmapMesh.end() )
				{
					Graphics::AbstractMesh* mesh = GenPrimitive( meshKey );
					if ( !mesh )
					{
						Graphics::Skeleton* skeleton = CoreGetSkeleton( meshKey );
						mesh = skeleton->GetMesh();
					}
					return mesh;
				}
				return iter->second;
			}

			Graphics::AbstractMesh* ResourceManager::CoreGetMesh( const std::string& directory, const std::string& fileName )
			{
				std::string key = directory + fileName;
				auto iter = m_hashmapMesh.find( key );
				if ( iter == m_hashmapMesh.end() )
				{
					Graphics::Skeleton* skeleton = CoreGetSkeleton( directory, fileName );
					return skeleton->GetMesh();
				}

				return iter->second;
			}

			// * Get a Mesh using its Key
			Graphics::Skeleton* ResourceManager::CoreGetSkeleton( const std::string& meshKey )
			{
				auto iter = m_hashmapSkeleton.find( meshKey );
				if ( iter == m_hashmapSkeleton.end() )
					return CoreCreateSkeleton( meshKey );
				return iter->second;
			}

			Graphics::Skeleton* ResourceManager::CoreCreateSkeleton( const std::string& skeletonKey )
			{
				Graphics::Skeleton* skeleton = MeshLoader::LoadMesh( skeletonKey );
				if ( skeleton == nullptr )
					return nullptr;
				m_hashmapSkeleton[skeletonKey] = skeleton;
				Graphics::AbstractMesh* mesh = skeleton->GetMesh();
				mesh->SetKey( skeletonKey );
				CoreRegisterMesh( mesh );

				for ( int i = 0; i < skeleton->GetSubMeshCount(); ++i )
				{
					Graphics::SubMesh* bone = skeleton->GetSubMesh( i );
					bone->SetMesh( mesh );
					CoreRegisterMaterial( bone->GetMaterial() );
				}
				return skeleton;
			}

			Graphics::Skeleton* ResourceManager::CoreGetSkeleton( const std::string& directory, const std::string& fileName )
			{
				std::string key = directory + fileName;
				auto iter = m_hashmapSkeleton.find( key );
				if ( iter == m_hashmapSkeleton.end() )
					return CoreCreateSkeleton( directory, fileName );
				return iter->second;
			}
#pragma endregion

#pragma region Shader

			// * Get a Shader using its Key
			Graphics::AbstractShader* ResourceManager::CoreGetShader( const std::string& vertexShaderPath, const std::string& fragmentShaderPath )
			{
				std::string shaderKey = vertexShaderPath + fragmentShaderPath;
				auto iter = m_hashmapShader.find( shaderKey );
				if ( iter == m_hashmapShader.end() )
					return CoreCreateShader( vertexShaderPath, fragmentShaderPath );
				return iter->second;
			}

			// * Create a Shader based on its different shader files
			// * ShaderKey is the concatenation of every paths
			Graphics::AbstractShader* ResourceManager::CoreCreateShader( const std::string& vertexShaderPath, const std::string& fragmentShaderPath )
			{
				// TODO: change
				std::string shaderKey = vertexShaderPath + fragmentShaderPath;

				Graphics::AbstractShader* shader = Graphics::RendererFactory::CreateShader( vertexShaderPath.c_str(), fragmentShaderPath.c_str() );
				m_hashmapShader.insert( std::pair<const std::string, Graphics::AbstractShader *>( shaderKey, shader ) );
#				
				return shader;
			}
#pragma endregion

#pragma region Material

			// * Get a Material using its Key
			Graphics::AbstractMaterial* ResourceManager::CoreGetMaterial( const std::string& materialKey )
			{
				Graphics::AbstractMaterial* material;

				auto iter = m_hashmapMaterial.find( materialKey );

				if ( iter != m_hashmapMaterial.end() )
					material = iter->second;
				else
					return Graphics::RendererFactory::CreateMaterial();

				//Graphics::TCMMatMapContainer& matMapContainer = material->m_maps;

				// Use the TCMResourceManager's textures (contains UBO)
				/*if ( matMapContainer.m_ambientMap != nullptr )
					matMapContainer.m_ambientMap = GetTexture( matMapContainer.m_ambientMap->GetKey() );
				if ( matMapContainer.m_diffuseMap != nullptr )
					matMapContainer.m_diffuseMap = GetTexture( matMapContainer.m_diffuseMap->GetKey() );
				if ( matMapContainer.m_specularColorMap != nullptr )
					matMapContainer.m_specularColorMap = GetTexture( matMapContainer.m_specularColorMap->GetKey() );
				if ( matMapContainer.m_shininessMap != nullptr )
					matMapContainer.m_shininessMap = GetTexture( matMapContainer.m_shininessMap->GetKey() );
				if ( matMapContainer.m_bumpMap != nullptr )
					matMapContainer.m_bumpMap = GetTexture( matMapContainer.m_bumpMap->GetKey() );
				if ( matMapContainer.m_alphaMap != nullptr )
					matMapContainer.m_alphaMap = GetTexture( matMapContainer.m_alphaMap->GetKey() );
					*/
				return material;
			}

			void ResourceManager::CoreRegisterMaterial( Graphics::AbstractMaterial* material )
			{
				if ( material == nullptr )
					return;
				std::string key = material->GetName();

				auto iter = m_hashmapMaterial.find( key );
				if ( iter == m_hashmapMaterial.end() )
					m_hashmapMaterial[key] = material;

				CoreRegisterTexture( material->GetAmbientMap() );
				CoreRegisterTexture( material->GetDiffuseMap() );
				CoreRegisterTexture( material->GetSpecularColorMap() );
				CoreRegisterTexture( material->GetShininessMap() );
				CoreRegisterTexture( material->GetBumpMap() );
				CoreRegisterTexture( material->GetAlphaMap() );
			}
#pragma endregion

#pragma region Texture

			// * Create a Texture from the TCM::Engine::Graphics::AbstractTexture base
			// * Texture created is stored using its Key
			Graphics::AbstractTexture* ResourceManager::CoreCreateTexture( const int type, const int width, const int height, const uint8_t* texData ) const
			{
				return Graphics::RendererFactory::CreateTexture( type, width, height, texData );
			}

			void ResourceManager::CoreRegisterTexture( Graphics::AbstractTexture* texture )
			{
				if ( texture == nullptr )
					return;

				auto iter = m_hashmapTexture.find( texture->GetKey() );
				if ( iter == m_hashmapTexture.end() )
					m_hashmapTexture[texture->GetKey()] = texture;
			}

			// * Create a Texutre from a file
			// * Texture created is stored using its key
			// * Return the new texture that have been created
			Graphics::AbstractTexture* ResourceManager::CoreCreateTexture( const std::string& path )
			{
				int width, height, type;

				uint8_t* texData = stbi_load( path.c_str(), &width, &height, &type, STBI_rgb_alpha );

				if ( texData == nullptr )
				{
					TCMWARNING("Failed to load texture [" + std::string(path) + "]");
					return nullptr;
				}

				Graphics::AbstractTexture* texture = CoreCreateTexture( type, width, height, texData );
				texture->SetKey( path );

				m_hashmapTexture[path] = texture;

				if ( texData )
					stbi_image_free( static_cast<void *>(texData) );

				return texture;
			}

			// * Get a Texture using its Key
			Graphics::AbstractTexture* ResourceManager::CoreGetTexture( const std::string& textureKey )
			{
				auto iter = m_hashmapTexture.find( textureKey );
				if ( iter == m_hashmapTexture.end() )
					return CoreCreateTexture( textureKey );
				return iter->second;
			}
#pragma endregion
		}
	}
}
