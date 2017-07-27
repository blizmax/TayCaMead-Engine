#include "TCMAttenuatedLight.h"
#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMRenderer/Gizmo/Gizmo.h"
#include "TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		TCMAttenuatedLight::TCMAttenuatedLight()
			: m_gizmoMesh( Resources::ResourceManager::GetMesh( "Sphere 1.0 24 16" ) )
		{
			SetName( "TCMAttenuatedLight" );
		}

		void TCMAttenuatedLight::TCMPoolCreate()
		{
			Super::TCMPoolCreate();

			int width, height;
			float near, far;
			Graphics::RendererFactory::GetShadowParams( width, height, near, far );
			m_lightProjection = Math::Mat4::ProjMatrix( static_cast<float>(width), static_cast<float>(height), 45.0f, near, far ).Transposed();
		}

		void TCMAttenuatedLight::ResetParams()
		{
			TCMLightSource::ResetParams();

			m_data->m_cutOff = 0.f;
			m_data->m_outerCutOff = 0.f;
		}

		void TCMAttenuatedLight::OnDrawGizmos()
		{
			Debug::Gizmo::Color( m_data->m_diffuseColor );
			float radius = m_data->GetRadius();
			Debug::Gizmo::DrawWiredMesh( m_gizmoMesh,
			                             m_sceneObject->GetWorldPosition(),
			                             Math::Vec3(),
			                             Math::Vec3( radius, radius, radius ) );
		}

		void TCMAttenuatedLight::SetConstantAttenuation( const float value ) const
		{
			m_data->m_constantAttenuation = value;
		}

		void TCMAttenuatedLight::SetLinearAttenuation( const float value ) const
		{
			m_data->m_linearAttenuation = value;
		}

		void TCMAttenuatedLight::SetQuadraticAttenuation( const float value ) const
		{
			m_data->m_quadraticAttenuation = value;
		}
	}
}
