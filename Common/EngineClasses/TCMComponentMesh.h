#pragma once

#include <vector>

#include "TCMComponent.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "ObjectLoader/MatLoader.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief Structure containing every key for Resources
		*/
		typedef struct s_RenderFiles
		{
			s_RenderFiles()
				: m_meshKey( "" )
				, m_vertexShaderPath( "" )
				, m_fragmentShaderPath( "" )
			{
			}

			s_RenderFiles( const s_RenderFiles& other )
				: m_meshKey( other.m_meshKey )
				, m_vertexShaderPath( other.m_vertexShaderPath )
				, m_fragmentShaderPath( other.m_fragmentShaderPath )
			{
			}

			s_RenderFiles& operator=( const s_RenderFiles& other )
			{
				if ( this == &other )
					return *this;

				m_meshKey = other.m_meshKey;
				m_vertexShaderPath = other.m_vertexShaderPath;
				m_fragmentShaderPath = other.m_fragmentShaderPath;

				return *this;
			}

			/**
			* \brief The Mesh Key
			*/
			std::string m_meshKey;

			/**
			* \brief The Vertex Shader path
			*/
			std::string m_vertexShaderPath;

			/**
			* \brief The Fragment Shader path
			*/
			std::string m_fragmentShaderPath;
		} RenderFiles;

		struct MaterialFiles
		{
			std::string m_ambientMapPath;

			std::string m_diffuseMapPath;

			std::string m_specularMapPath;

			std::string m_shininessMapPath;

			std::string m_bumpMapPath;

			std::string m_alphaMapPath;
		};

		struct MeshCache
		{
			MeshCache();
			~MeshCache();

			std::string m_meshKeyCached;

			std::string m_vertexShaderPathCached;

			std::string m_fragmentShaderPathCached;

			Graphics::AbstractMaterial* m_materialCached;

			std::string m_ambientPathCached;

			std::string m_diffusePathCached;

			std::string m_specularPathCached;

			std::string m_shininessPathCached;

			std::string m_bumpPathCached;

			std::string m_alphaPathCached;
		};

		// * Base class for every drawable component
		class ENGINECLASSES_DLL_EXPORT TCMComponentMesh : public TCMComponent
		{
			TCM_CLASS( TCMComponentMesh, TCMComponent );

			TCM_SERIALIZE(
				m_renderFiles->m_meshKey
				, m_renderFiles->m_vertexShaderPath
				, m_renderFiles->m_fragmentShaderPath
				, m_materialFiles->m_ambientMapPath
				, m_materialFiles->m_diffuseMapPath
				, m_materialFiles->m_specularMapPath
				, m_materialFiles->m_shininessMapPath
				, m_materialFiles->m_bumpMapPath
				, m_material->_ambientColor
				, m_material->_diffuseColor
				, m_material->_specularColor
				, m_material->_emissiveColor
				, m_material->_opticalDensity
				, m_material->_shininess
				, m_material->_sharpness );

		public:
			TCMComponentMesh();
			virtual ~TCMComponentMesh();

			/**
			* \brief Pseudo-constructor used instanciated by pool
			*/
			virtual void TCMPoolCreate() override;

			/**
			* \brief Pseudo-destructor used when released by pool
			*/
			virtual void TCMPoolRelease() override;

			void Construct( Graphics::AbstractMesh* mesh );
			/**
			* \brief Sets the key of the mesh
			* \param meshKey _IN_ The key to be used
			*/
			void SetMeshKey( const std::string& meshKey );

			/**
			* \brief Set the mesh explicitly
			*/
			void SetMesh( Graphics::AbstractMesh* mesh );

			/**
			* \brief Sets the Shader of this object
			* \param vertex _IN_ Path to the Vertex Shader
			* \param fragment _IN_ Path to the Fragment Shader
			*/
			void SetShader( const std::string& vertex, const std::string& fragment );

			/**
			* \brief Set the shader explicitly
			*/
			void SetShader( Graphics::AbstractShader* shader );

			/**
			* \brief Sets the Material of this object
			* \param material _IN_ Material to be set
			*/
			void SetMaterial( Graphics::AbstractMaterial* material );

			/**
			* \brief Set the material explicitly
			*/
			void SetMaterialKey( const std::string& materialkey );

			Graphics::AbstractMaterial* GetMaterial() const;

			/**
			* \brief Get the current name
			* \return The mesh's name
			*/
			std::string GetMeshName() const;

			/**
			* \brief Get the component's RenderData
			* \return The component's RenderData
			*/
			Graphics::RenderData GetRenderData() const;

			/**
			* \brief Initialization method called after creation
			*/
			virtual void Init() override;

			void TCMEditorUpdate( const float deltaTime ) override;

			void OnWorldMatrixChanged( const Math::Mat4& mat ) override;

			void OnEnable() override;

			void OnDisable() override;

		protected:

			/**
			* \brief The RenderFile struct of this component
			*/
			RenderFiles* m_renderFiles;

			/**
			* \brief The RenderData struct of this component
			*/
			Graphics::RenderData m_renderData;

			/**
			* \brief Refresh the component's RenderData
			*/
			void RefreshRenderData();

		private:

			/**
			* \brief Some extra data to be sent if needed
			*/
			Graphics::ExtraRenderData* m_extraToSend;

			MaterialFiles* m_materialFiles;

			Graphics::AbstractMaterial* m_material;

#pragma  region Editor Cache

			MeshCache* m_cache;
#pragma endregion
		};
	}
}
