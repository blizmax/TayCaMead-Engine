#include "TCMComponentDrawable2D.h"
#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMSceneObject.h"
#include "TCMRenderer/Rendering/RenderGraph.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMComponentDrawable2D )

		TCMComponentDrawable2D::TCMComponentDrawable2D()
		{
			SetName( "TCMComponentDrawable2D" );
			m_data = new Graphics::SpriteData;
		}

		TCMComponentDrawable2D::~TCMComponentDrawable2D()
		{
			if ( m_data )
			{
				delete m_data;
				m_data = nullptr;
			}
		}

		void TCMComponentDrawable2D::TCMPoolCreate()
		{
			TCMComponent::TCMPoolCreate();

			m_data->Reset();
			m_data->m_enabled = true;
			Graphics::RenderGraph::RegisterSprite( m_data );
		}

		void TCMComponentDrawable2D::TCMPoolRelease()
		{
			TCMComponent::TCMPoolRelease();

			m_data->Reset();
			m_data->m_enabled = false;
			Graphics::RenderGraph::UnregisterSprite( m_data );
		}

		void TCMComponentDrawable2D::Construct( const std::string& filepath )
		{
			m_data->SetTexture( Resources::ResourceManager::GetTexture( filepath ) );
		}

		void TCMComponentDrawable2D::Construct( Graphics::AbstractTexture* sprite )
		{
			m_data->SetTexture( sprite );
		}

		void TCMComponentDrawable2D::OnWorldMatrixChanged( const Math::Mat4& mat )
		{
			CalculModelMat();
		}

		void TCMComponentDrawable2D::CalculModelMat()
		{
			Math::Mat4 m = Math::Mat4::Identity();

			m *= Math::Mat4::CreateTranslation( m_sceneObject->GetWorldPosition() );
			m *= Math::Mat4::CreateTranslation( m_data->GetTexture()->GetWidth() * 0.5f, m_data->GetTexture()->GetHeight() * 0.5f, 0.0f );
			m *= Math::Mat4::CreateRotation( m_sceneObject->GetWorldRotation() );
			m *= Math::Mat4::CreateScale( m_sceneObject->GetWorldScale() );

			m_data->SetWorldMat( m );
		}
	}
}
