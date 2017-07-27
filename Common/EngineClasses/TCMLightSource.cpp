#include "TCMLightSource.h"
#include "TCMSceneObject.h"

#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TCMRenderer/Rendering/RenderGraph.h"

namespace TCM
{
	namespace Engine
	{
		TCMLightSource::TCMLightSource()
			: TCMComponent()
			, m_data( Graphics::RendererFactory::CreateLight() )
			, m_lightCached( Graphics::RendererFactory::CreateLight() )
		{
			SetName( "TCMLightSource" );
		}

		TCMLightSource::~TCMLightSource()
		{
			if ( m_data )
			{
				delete m_data;
				m_data = nullptr;
			}

			if ( m_lightCached )
			{
				delete m_lightCached;
				m_lightCached = nullptr;
			}
		}

		void TCMLightSource::TCMPoolCreate()
		{
			Super::TCMPoolCreate();

			ResetParams();
			Graphics::RenderGraph::RegisterLight( m_data );
			m_data->m_enabled = true;
		}

		void TCMLightSource::TCMPoolRelease()
		{
			m_data->m_enabled = false;
			Graphics::RenderGraph::UnregisterLight( m_data );

			Super::TCMPoolRelease();
		}

		void TCMLightSource::TCMEditorUpdate( const float deltaTime )
		{
			if ( *m_lightCached != *m_data )
			{
				m_data->UpdateData();
				*m_lightCached = *m_data;
			}
		}

		void TCMLightSource::ResetParams()
		{
			m_data->m_lightSpace = Math::Mat4::Identity();
			m_data->m_ambientColor = Math::ColorRGB::White();
			m_data->m_diffuseColor = Math::ColorRGB::White();
			m_data->m_specularColor = Math::ColorRGB::White();

			m_data->m_constantAttenuation = 2.0f;
			m_data->m_linearAttenuation = 1.0f;
			m_data->m_quadraticAttenuation = 1.0f;

			m_data->m_cutOff = -1.f;
			m_data->m_outerCutOff = -1.f;
			UpdateData();
		}

		bool TCMLightSource::IsDirectionnal() const
		{
			return GetDirection().w == 0.0f;
		}

		Math::ColorRGB TCMLightSource::GetAmbientColor() const
		{
			return m_data->m_ambientColor;
		}

		Math::ColorRGB TCMLightSource::GetDiffuseColor() const
		{
			return m_data->m_diffuseColor;
		}

		Math::ColorRGB TCMLightSource::GetSpecularColor() const
		{
			return m_data->m_specularColor;
		}

		void TCMLightSource::SetAmbientColor( Math::ColorRGB color ) const
		{
			if ( !m_data )
				return;
			m_data->m_ambientColor = color;
			UpdateData();
		}

		void TCMLightSource::SetDiffuseColor( Math::ColorRGB color ) const
		{
			if ( !m_data )
				return;
			m_data->m_diffuseColor = color;
			UpdateData();
		}

		void TCMLightSource::SetSpecularColor( Math::ColorRGB color ) const
		{
			if ( !m_data )
				return;
			m_data->m_specularColor = color;
			UpdateData();
		}

		float TCMLightSource::GetConstantAttenuation() const
		{
			return m_data->m_constantAttenuation;
		}

		float TCMLightSource::GetLinearAttenuation() const
		{
			return m_data->m_linearAttenuation;
		}

		float TCMLightSource::GetQuadraticAttenuation() const
		{
			return m_data->m_quadraticAttenuation;
		}

		float TCMLightSource::GetCutOff() const
		{
			return m_data->m_cutOff;
		}

		float TCMLightSource::GetOuterCutOff() const
		{
			return m_data->m_outerCutOff;
		}

		Graphics::LightData* TCMLightSource::GetLightData() const
		{
			return m_data;
		}

		void TCMLightSource::OnPositionChanged( const Math::Vec3& position )
		{
			UpdateData();
		}

		void TCMLightSource::OnRotationChanged( const Math::Vec3& rotation )
		{
			UpdateData();
		}

		void TCMLightSource::UpdateData() const
		{
			if ( !m_sceneObject || !m_data )
				return;
			m_data->m_lightSpace = m_lightProjection * GetWorldMatrix();
			m_data->m_position = m_sceneObject->GetWorldPosition();
			m_data->m_direction = GetDirection();
			m_data->UpdateData();
		}

		void TCMLightSource::OnEnable()
		{
			if ( m_data )
				m_data->m_enabled = true;
		}

		void TCMLightSource::OnDisable()
		{
			if ( m_data )
				m_data->m_enabled = false;
		}

		void TCMLightSource::OnWorldMatrixChanged( const Math::Mat4& mat )
		{
			UpdateData();
		}
	}
}
