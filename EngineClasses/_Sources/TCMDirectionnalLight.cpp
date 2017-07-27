#include "TCMDirectionnalLight.h"
#include "TCMSceneObject.h"
#include "TCMRenderer/Rendering/RendererFactory.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMDirectionnalLight )

		TCMDirectionnalLight::TCMDirectionnalLight()
		{
			SetName( "TCMDirectionnalLight" );
		}

		void TCMDirectionnalLight::TCMPoolCreate()
		{
			Super::TCMPoolCreate();

			int width, height;
			float near, far;
			Graphics::RendererFactory::GetShadowParams( width, height, near, far );
			m_lightProjection = Math::Mat4::OrthoProj( -10.0f, 10.0f, -10.0f, 10.0f, near, far ).Transposed();
		}

		Math::Vec4 TCMDirectionnalLight::GetDirection() const
		{
			return Math::Vec4( m_sceneObject->Forward(), 0.0f );
		}
	}
}
