#pragma once

#include <vector>

#include "TCMComponent.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief Structure containing every key for Resources
		*/
		typedef struct s_SubMeshData
		{
			s_SubMeshData()
				: m_meshKey( "" )
				, m_materialKey( "" )
				, m_vertexShaderPath( "" )
				, m_fragmentShaderPath( "" )
				, m_indicesOffset( 0 )
				, m_indicesCount( 0 )
			{
			}

			s_SubMeshData( const s_SubMeshData& other )
				: m_meshKey( other.m_meshKey )
				, m_materialKey( other.m_materialKey )
				, m_vertexShaderPath( other.m_vertexShaderPath )
				, m_fragmentShaderPath( other.m_fragmentShaderPath )
				, m_indicesOffset( other.m_indicesOffset )
				, m_indicesCount( other.m_indicesCount )
			{
			}

			s_SubMeshData& operator=( const s_SubMeshData& other )
			{
				if ( this == &other )
					return *this;

				m_meshKey = other.m_meshKey;
				m_materialKey = other.m_materialKey;
				m_vertexShaderPath = other.m_vertexShaderPath;
				m_fragmentShaderPath = other.m_fragmentShaderPath;

				return *this;
			}

			/**
			* \brief The Mesh Key
			*/
			std::string m_meshKey;

			/**
			* \brief The Material Key
			*/
			std::string m_materialKey;

			/**
			* \brief The Vertex Shader path
			*/
			std::string m_vertexShaderPath;

			/**
			* \brief The Fragment Shader path
			*/
			std::string m_fragmentShaderPath;

			uint64_t m_indicesOffset;

			uint64_t m_indicesCount;

			Math::Bounds::Sphere m_sphere;
		} SubMeshData;

		// * Base class for every drawable component
		class ENGINECLASSES_DLL_EXPORT TCMComponentSubMesh : public TCMComponent
		{
			TCM_CLASS( TCMComponentSubMesh, TCMComponent );

			TCM_SERIALIZE( m_subMeshData->m_vertexShaderPath
				, m_subMeshData->m_fragmentShaderPath
			);

			TCM_SERIALIZE_READONLY( m_subMeshData->m_meshKey
				, m_subMeshData->m_materialKey )

			TCM_SERIALIZE_HIDDEN( m_subMeshData->m_indicesOffset
				, m_subMeshData->m_indicesCount
				, m_subMeshData->m_sphere)

		public:
			TCMComponentSubMesh();
			virtual ~TCMComponentSubMesh();

			/**
			* \brief Pseudo-constructor used instanciated by pool
			*/
			virtual void TCMPoolCreate() override;

			/**
			* \brief Pseudo-destructor used when released by pool
			*/
			virtual void TCMPoolRelease() override;

			void Construct( Graphics::SubMesh* submesh );

			/**
			* \brief Set all datas from the SubMesh
			* \param submesh _IN_ SubMesh used
			*/
			void SetSubMesh( Graphics::SubMesh* submesh );

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
			* \brief Get the current name
			* \return The mesh's name
			*/
			std::string GetMeshName() const;

			/**
			* \brief Get the current material name
			* \return The material's name
			*/
			std::string GetMaterialName() const;

			/**
			* \brief Get the component's RenderData
			* \return The component's RenderData
			*/
			Graphics::RenderData GetRenderData() const;

			/**
			* \brief Initialization method called after creation
			*/
			virtual void Init() override;

			void OnWorldMatrixChanged( const Math::Mat4& mat ) override;
			void OnEnable() override;
			void OnDisable() override;

		protected:

			/**
			* \brief The RenderFile struct of this component
			*/
			SubMeshData* m_subMeshData;

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
		};
	}
}
