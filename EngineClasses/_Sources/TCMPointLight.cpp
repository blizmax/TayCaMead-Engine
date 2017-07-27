#include "TCMPointLight.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMPointLight )

		TCMPointLight::TCMPointLight()
		{
			SetName( "TCMPointLight" );
		}

		Math::Vec4 TCMPointLight::GetDirection() const
		{
			return Math::Vec4( 0.0f, 0.0f, 0.0f, 1.0f );
		}
	}
}
