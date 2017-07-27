#include "TCMComponentSubMesh.h"

#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMComponentSubMesh )

		TCMComponentSubMesh::TCMComponentSubMesh()
			: m_subMeshData( nullptr )
			, m_extraToSend( nullptr )
		{
			SetName( "TCMComponentSubMesh" );
			m_subMeshData = new SubMeshData();
			m_extraToSend = new Graphics::ExtraRenderData();
			m_renderData.m_extra = m_extraToSend;
		}

		TCMComponentSubMesh::~TCMComponentSubMesh()
		{
			if ( m_subMeshData )
			{
				delete m_subMeshData;
				m_subMeshData = nullptr;
			}

			if ( m_extraToSend )
			{
				delete m_extraToSend;
				m_extraToSend = nullptr;
			}
		}

		void TCMComponentSubMesh::TCMPoolCreate()
		{
			TCMComponent::TCMPoolCreate();

			//m_drawableName = "TCMComponentDrawable" + std::to_string( GetID() );

			m_extraToSend->m_isSelected = false;

			m_subMeshData->m_meshKey.assign( "TCMComponentSubMesh" + std::to_string( GetID() ) );
			m_subMeshData->m_vertexShaderPath.assign( "../Common/TCMRenderer/OpenGLShaders/base.vert" );
			m_subMeshData->m_fragmentShaderPath.assign( "../Common/TCMRenderer/OpenGLShaders/complex.frag" );
			m_subMeshData->m_indicesCount = 0;
			m_subMeshData->m_indicesOffset = 0;
			m_subMeshData->m_sphere = Math::Bounds::Sphere();
		}

		void TCMComponentSubMesh::TCMPoolRelease()
		{
			Graphics::RenderGraph::UnregisterRenderData( &m_renderData );
			m_renderData.m_enabled = false;

			if ( m_extraToSend )
			{
				// Nothing to do
			}

			TCMComponent::TCMPoolRelease();
		}

		void TCMComponentSubMesh::Construct( Graphics::SubMesh* submesh )
		{
			SetSubMesh( submesh );
		}

		void TCMComponentSubMesh::SetSubMesh( Graphics::SubMesh* submesh )
		{
			if ( !submesh )
				return;
			Graphics::AbstractMesh* mesh = submesh->GetMesh();
			m_renderData.SetMesh( mesh, submesh->GetSphere() );
			m_subMeshData->m_meshKey = mesh->GetKey();

			Graphics::AbstractMaterial* material = submesh->GetMaterial();
			m_subMeshData->m_materialKey = material->GetName();
			m_renderData.SetMaterial( material );

			m_subMeshData->m_indicesOffset = submesh->GetOffset();
			m_subMeshData->m_indicesCount = submesh->GetCount();
			m_subMeshData->m_sphere = submesh->GetSphere();

			m_renderData.SetIndicesOffset( submesh->GetOffset() );
			m_renderData.SetIndicesCount( submesh->GetCount() );
		}

		std::string TCMComponentSubMesh::GetMeshName() const
		{
			return m_subMeshData->m_meshKey;
		}

		std::string TCMComponentSubMesh::GetMaterialName() const
		{
			return m_subMeshData->m_materialKey;
		}

		Graphics::RenderData TCMComponentSubMesh::GetRenderData() const
		{
			return m_renderData;
		}

		void TCMComponentSubMesh::Init()
		{
			TCMComponent::Init();

			RefreshRenderData();

			Graphics::RenderGraph::RegisterRenderData( &m_renderData );
			m_renderData.m_enabled = true;
			m_renderData.m_caller = this;
		}

		void TCMComponentSubMesh::RefreshRenderData()
		{
			if ( m_subMeshData->m_sphere.radius > 0 && m_subMeshData->m_indicesCount > 0 )
			{
				m_renderData.SetMesh( Resources::ResourceManager::GetMesh( m_subMeshData->m_meshKey ), m_subMeshData->m_sphere );
				m_renderData.SetIndicesCount( m_subMeshData->m_indicesCount );
				m_renderData.SetIndicesOffset( m_subMeshData->m_indicesOffset );
			}
			else
			{
				m_renderData.SetMesh( Resources::ResourceManager::GetMesh( m_subMeshData->m_meshKey ), false );
			}
			m_renderData.SetMaterial( Resources::ResourceManager::GetMaterial( m_subMeshData->m_materialKey ) );
			m_renderData.SetShader( Resources::ResourceManager::GetShader( m_subMeshData->m_vertexShaderPath,
			                                                               m_subMeshData->m_fragmentShaderPath ) );
		}

		void TCMComponentSubMesh::SetShader( const std::string& vertex, const std::string& fragment )
		{
			m_subMeshData->m_vertexShaderPath = vertex;
			m_subMeshData->m_fragmentShaderPath = fragment;

			m_renderData.SetShader( Resources::ResourceManager::GetShader( m_subMeshData->m_vertexShaderPath,
			                                                               m_subMeshData->m_fragmentShaderPath ) );
		}

		void TCMComponentSubMesh::SetShader( Graphics::AbstractShader* shader )
		{
			m_renderData.SetShader( shader );
		}

		void TCMComponentSubMesh::OnWorldMatrixChanged( const Math::Mat4& mat )
		{
			if ( !m_sceneObject )
				return;
			TCMSceneObject* parent = m_sceneObject->GetParent();
			Math::Mat4 parentMat = Math::Mat4::Identity();
			if ( parent )
				parentMat = parent->GetWorldMatrix();
			m_renderData.SetWorldMatrix( parentMat, m_sceneObject->GetLocaleMatrix(),
			                             m_sceneObject->GetWorldPosition(),
			                             m_sceneObject->GetWorldScale() );
		}

		void TCMComponentSubMesh::OnEnable()
		{
			m_renderData.m_enabled = true;
		}

		void TCMComponentSubMesh::OnDisable()
		{
			m_renderData.m_enabled = false;
		}
	}
}
