#pragma once

#include "TCMResourceManager/ResourceManagerDefines.h"

#include <unordered_map>

#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"

namespace TCM
{
	namespace Engine
	{
		namespace Resources
		{
			/**
			 * \brief Class used for cache and keeping data shared and unique when needed
			 */
			class ResourceManager
			{
			public:
				virtual ~ResourceManager();

				/**
				* \brief Get a Mesh using its Key
				* \param meshKey _IN_ Mesh key
				* \return The Mesh associated with the key\n
				*		   Returns nullptr if the key is not associated to any Mesh
				*/
				static TCMRESOURCEMANAGER_DLL Graphics::AbstractMesh* GetMesh( const std::string& meshKey );

				static TCMRESOURCEMANAGER_DLL Graphics::AbstractMesh* GetMesh( const std::string& directory, const std::string& fileName );

				static TCMRESOURCEMANAGER_DLL bool RegisterMesh( Graphics::AbstractMesh* mesh );

				static TCMRESOURCEMANAGER_DLL Graphics::Skeleton* GetSkeleton( const std::string& meshKey );

				static TCMRESOURCEMANAGER_DLL Graphics::Skeleton* GetSkeleton( const std::string& directory, const std::string& fileName );

				static TCMRESOURCEMANAGER_DLL Graphics::AbstractShader* GetShader( const std::string& vertexShaderPath, const std::string& fragmentShaderPath );

				/**
				* \brief Get a Material using its Key
				* \param materialKey _IN_ Material key
				* \return The Material associated with the key\n
				*		   Returns nullptr if the key is not associated to any Material
				*/
				static TCMRESOURCEMANAGER_DLL Graphics::AbstractMaterial* GetMaterial( const std::string& materialKey );

				/**
				* \brief Get a Texture using its Key
				* \param textureKey _IN_ Texture key
				* \return The Texture associated with the key\n
				*		   Returns nullptr if the key is not associated to any Texture
				*/
				static TCMRESOURCEMANAGER_DLL Graphics::AbstractTexture* GetTexture( const std::string& textureKey );

			protected:
				ResourceManager() = default;

				/**
				* \brief Get a Mesh using its Key
				* \param meshKey _IN_ Mesh key
				* \return The Mesh associated with the key\n
				*		   Returns nullptr if the key is not associated to any Mesh
				*/
				Graphics::Skeleton* CoreGetSkeleton( const std::string& meshKey );

				Graphics::Skeleton* CoreCreateSkeleton( const std::string& skeletonKey );

				Graphics::Skeleton* CoreGetSkeleton( const std::string& directory, const std::string& fileName );

				Graphics::Skeleton* CoreCreateSkeleton( const std::string& directory, const std::string& fileName );

#pragma region Mesh

				/**
				 * \brief Get a Mesh using its Key
				 * \param meshKey _IN_ Mesh key
				 * \return The Mesh associated with the key\n
				 *		   Returns nullptr if the key is not associated to any Mesh
				 */
				Graphics::AbstractMesh* CoreGetMesh( const std::string& meshKey );

				Graphics::AbstractMesh* CoreGetMesh( const std::string& directory, const std::string& fileName );

				bool CoreRegisterMesh( Graphics::AbstractMesh* mesh );

				enum class PrimitiveForm : uint8_t
				{
					SPHERE = 0,
					TUBE,
					CONE,
					TORUS,
					CAPSULE,
					PLANE,
					BOX,
					COUNT
				};

				PrimitiveForm GetForm( const std::string& key ) const;

				Graphics::AbstractMesh* GenPrimitive( const std::string& key ) const;

#pragma endregion

#pragma region Shader
				/**
				 * \brief Get a Shader using its key
				 * \param vertexShaderPath _IN_ Shader path
				 * \param fragmentShaderPath _IN_ Fragment Shader path
				 * \return The Shader associated with the key\n
				 *		   Returns nullptr if the key is not associated to any Shader
				 */
				Graphics::AbstractShader* CoreGetShader( const std::string& vertexShaderPath, const std::string& fragmentShaderPath );

				Graphics::AbstractShader* CoreCreateShader( const std::string& vertexShaderPath, const std::string& fragmentShaderPath );
#pragma endregion

#pragma region Material

				/**
				 * \brief Get a Material using its Key
				 * \param materialKey _IN_ Material key
				 * \return The Material associated with the key\n
				 *		   Returns nullptr if the key is not associated to any Material
				 */
				Graphics::AbstractMaterial* CoreGetMaterial( const std::string& materialKey );

				void CoreRegisterMaterial( Graphics::AbstractMaterial* material );
#pragma endregion

#pragma region Texture

				/**
				 * \brief Get a Texture using its Key
				 * \param textureKey _IN_ Texture key
				 * \return The Texture associated with the key\n
				 *		   Returns nullptr if the key is not associated to any Texture
				 */
				Graphics::AbstractTexture* CoreGetTexture( const std::string& textureKey );

				Graphics::AbstractTexture* CoreCreateTexture( const std::string& textureKey );

				Graphics::AbstractTexture* CoreCreateTexture( const int type, const int width, const int height, const uint8_t* texData ) const;

				void CoreRegisterTexture( Graphics::AbstractTexture* texture );
#pragma endregion

#pragma region Hashmaps

				/**
				* \brief Hashmap containing every Mesh that has been set in the TCMResourceManager
				*/
				std::unordered_map<std::string, Graphics::AbstractMesh *> m_hashmapMesh;

				/**
				* \brief Hashmap containing every Skeleton that has been set in the TCMResourceManager
				*/
				std::unordered_map<std::string, Graphics::Skeleton *> m_hashmapSkeleton;

				/**
				 * \brief Hashmap containing every Shader that has been set in the TCMResourceManager
				 */
				std::unordered_map<std::string, Graphics::AbstractShader *> m_hashmapShader;

				/**
				 * \brief Hashmap containing every Texture that has been set in the TCMResourceManager
				 */
				std::unordered_map<std::string, Graphics::AbstractTexture *> m_hashmapTexture;

				/**
				 * \brief Hashmap containing every Material that has been set in the TCMResourceManager
				 */
				std::unordered_map<std::string, Graphics::AbstractMaterial *> m_hashmapMaterial;
#pragma endregion

				/**
				 * \brief Abstract RenderData used for rendering an object
				 */
				Graphics::RenderData m_renderData;

				/**
				 * \brief Initialize default values
				 */
				void Init();

			private:
				/**
				 * \brief Get the unique instance of the TCMResourceManager
				 * \return A reference to the TCMResourceManager
				 */
				static ResourceManager& GetInstance();
			};
		}
	}
}
