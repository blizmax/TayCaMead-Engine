#include "TCMComponentMesh.h"

#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMSceneObject.h"
#include "TCMRenderer/Rendering/RendererFactory.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMComponentMesh )

		MeshCache::MeshCache() : m_materialCached( Graphics::RendererFactory::CreateMaterial() )
		{
		}

		MeshCache::~MeshCache()
		{
			if ( m_materialCached )
			{
				delete m_materialCached;
				m_materialCached = nullptr;
			}
		}

		TCMComponentMesh::TCMComponentMesh()
			: m_renderFiles( new RenderFiles() )
			, m_extraToSend( new Graphics::ExtraRenderData() )
			, m_materialFiles( new MaterialFiles() )
			, m_material( Graphics::RendererFactory::CreateMaterial() )
			, m_cache( new MeshCache() )
		{
			SetName( "TCMComponentMesh" );
			m_renderData.m_extra = m_extraToSend;

			m_renderData.SetMaterial( m_material );
		}

		TCMComponentMesh::~TCMComponentMesh()
		{
			if ( m_renderFiles )
			{
				delete m_renderFiles;
				m_renderFiles = nullptr;
			}

			if ( m_extraToSend )
			{
				delete m_extraToSend;
				m_extraToSend = nullptr;
			}

			if ( m_materialFiles )
			{
				delete m_materialFiles;
				m_materialFiles = nullptr;
			}

			if ( m_material )
			{
				delete m_material;
				m_material = nullptr;
			}

			if ( m_cache )
			{
				delete m_cache;
				m_cache = nullptr;
			}
		}

		void TCMComponentMesh::TCMPoolCreate()
		{
			TCMComponent::TCMPoolCreate();

			//m_drawableName = "TCMComponentMesh" + std::to_string( GetID() );

			m_extraToSend->m_isSelected = false;

			m_renderFiles->m_meshKey.assign( "TCMComponentMesh" + std::to_string( GetID() ) );
			m_renderFiles->m_vertexShaderPath.assign( "../Common/TCMRenderer/OpenGLShaders/base.vert" );
			m_renderFiles->m_fragmentShaderPath.assign( "../Common/TCMRenderer/OpenGLShaders/complex.frag" );
		}

		void TCMComponentMesh::TCMPoolRelease()
		{
			Graphics::RenderGraph::UnregisterRenderData( &m_renderData );
			m_renderData.m_enabled = false;

			if ( m_extraToSend )
			{
				// Nothing to do
			}

			TCMComponent::TCMPoolRelease();
		}

		void TCMComponentMesh::Construct( Graphics::AbstractMesh* mesh )
		{
			if ( mesh )
				SetMesh( mesh );
		}

		void TCMComponentMesh::SetMeshKey( const std::string& meshKey )
		{
			m_renderFiles->m_meshKey = meshKey;
			Graphics::AbstractMesh* mesh = Resources::ResourceManager::GetMesh( m_renderFiles->m_meshKey );
			m_renderData.SetMesh( mesh );
		}

		void TCMComponentMesh::SetMesh( Graphics::AbstractMesh* mesh )
		{
			bool inserted = Resources::ResourceManager::RegisterMesh( mesh );
			if ( !inserted )
			{
				std::string key = mesh->GetKey();
				if ( mesh )
					delete mesh;
				SetMeshKey( key );
			}
			else
			{
				m_renderData.SetMesh( mesh );
				m_renderFiles->m_meshKey = mesh->GetKey();
			}
		}

		std::string TCMComponentMesh::GetMeshName() const
		{
			return m_renderFiles->m_meshKey;
		}

		Graphics::RenderData TCMComponentMesh::GetRenderData() const
		{
			return m_renderData;
		}

		void TCMComponentMesh::Init()
		{
			TCMComponent::Init();

			m_material->SetAmbientMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_ambientMapPath ) );
			m_material->SetDiffuseMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_diffuseMapPath ) );
			m_material->SetSpecularColorMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_specularMapPath ) );
			m_material->SetShininessMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_shininessMapPath ) );
			m_material->SetBumpMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_bumpMapPath ) );
			m_material->SetAlphaMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_alphaMapPath ) );

			m_material->UpdateData();

			RefreshRenderData();

			Graphics::RenderGraph::RegisterRenderData( &m_renderData );
			m_renderData.m_enabled = true;
			m_renderData.m_caller = this;
		}

		void TCMComponentMesh::TCMEditorUpdate( const float deltaTime )
		{
			if ( m_renderFiles->m_meshKey != m_cache->m_meshKeyCached )
			{
				SetMeshKey( m_renderFiles->m_meshKey );
				m_cache->m_meshKeyCached = m_renderFiles->m_meshKey;
			}

			if ( m_cache->m_vertexShaderPathCached != m_renderFiles->m_vertexShaderPath || m_cache->m_fragmentShaderPathCached != m_renderFiles->m_fragmentShaderPath )
			{
				SetShader( m_renderFiles->m_vertexShaderPath, m_renderFiles->m_fragmentShaderPath );
				m_cache->m_vertexShaderPathCached = m_renderFiles->m_vertexShaderPath;
				m_cache->m_fragmentShaderPathCached = m_renderFiles->m_fragmentShaderPath;
			}

			if ( *m_cache->m_materialCached != *m_material )
			{
				m_material->UpdateData();
				*m_cache->m_materialCached = *m_material;
			}

			if ( m_cache->m_ambientPathCached != m_materialFiles->m_ambientMapPath )
			{
				m_material->SetAmbientMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_ambientMapPath ) );
				m_cache->m_ambientPathCached = m_materialFiles->m_ambientMapPath;
			}

			if ( m_cache->m_diffusePathCached != m_materialFiles->m_diffuseMapPath )
			{
				m_material->SetDiffuseMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_diffuseMapPath ) );
				m_cache->m_diffusePathCached = m_materialFiles->m_diffuseMapPath;
			}

			if ( m_cache->m_specularPathCached != m_materialFiles->m_specularMapPath )
			{
				m_material->SetSpecularColorMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_specularMapPath ) );
				m_cache->m_specularPathCached = m_materialFiles->m_specularMapPath;
			}

			if ( m_cache->m_shininessPathCached != m_materialFiles->m_shininessMapPath )
			{
				m_material->SetShininessMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_shininessMapPath ) );
				m_cache->m_shininessPathCached = m_materialFiles->m_shininessMapPath;
			}

			if ( m_cache->m_bumpPathCached != m_materialFiles->m_bumpMapPath )
			{
				m_material->SetBumpMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_bumpMapPath ) );
				m_cache->m_bumpPathCached = m_materialFiles->m_bumpMapPath;
			}

			if ( m_cache->m_alphaPathCached != m_materialFiles->m_alphaMapPath )
			{
				m_material->SetAlphaMap( Resources::ResourceManager::GetTexture( m_materialFiles->m_alphaMapPath ) );
				m_cache->m_alphaPathCached = m_materialFiles->m_alphaMapPath;
			}
		}

		void TCMComponentMesh::RefreshRenderData()
		{
			m_renderData.SetMesh( Resources::ResourceManager::GetMesh( m_renderFiles->m_meshKey ) );
			m_renderData.SetShader( Resources::ResourceManager::GetShader( m_renderFiles->m_vertexShaderPath,
			                                                               m_renderFiles->m_fragmentShaderPath ) );
		}

		void TCMComponentMesh::SetShader( const std::string& vertex, const std::string& fragment )
		{
			m_renderFiles->m_vertexShaderPath = vertex;
			m_renderFiles->m_fragmentShaderPath = fragment;

			m_renderData.SetShader( Resources::ResourceManager::GetShader( m_renderFiles->m_vertexShaderPath,
			                                                               m_renderFiles->m_fragmentShaderPath ) );
		}

		void TCMComponentMesh::SetShader( Graphics::AbstractShader* shader )
		{
			m_renderData.SetShader( shader );
		}

		void TCMComponentMesh::SetMaterial( Graphics::AbstractMaterial* material )
		{
			if ( material == nullptr )
				return;
			m_material->CopyData( *material );
		}

		void TCMComponentMesh::SetMaterialKey( const std::string& materialkey )
		{
			Graphics::AbstractMaterial* material = Resources::ResourceManager::GetMaterial( materialkey );
			if ( material == nullptr )
				return;
			m_material->CopyData( *material );
		}

		Graphics::AbstractMaterial* TCMComponentMesh::GetMaterial() const
		{
			return m_material;
		}

		void TCMComponentMesh::OnWorldMatrixChanged( const Math::Mat4& mat )
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

		void TCMComponentMesh::OnEnable()
		{
			m_renderData.m_enabled = true;
		}

		void TCMComponentMesh::OnDisable()
		{
			m_renderData.m_enabled = false;
		}
	}
}
