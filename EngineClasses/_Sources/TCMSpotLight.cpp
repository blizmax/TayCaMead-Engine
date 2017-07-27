#include "TCMSpotLight.h"

#include "TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMSpotLight )

		TCMSpotLight::TCMSpotLight()
		{
			SetName( "TCMSpotLight" );
		}

		Math::Vec4 TCMSpotLight::GetDirection() const
		{
			return Math::Vec4( m_sceneObject->Forward(), 1.0f );
		}

		void TCMSpotLight::SetSpotLightValues( const float& cutOff, const float& outerCutOff ) const
		{
			SetCutOff( cutOff );
			SetOuterCutOff( outerCutOff );
		}

		void TCMSpotLight::SetCutOff( const float& degrees ) const
		{
			m_data->m_cutOff = degrees;
			UpdateData();
		}

		void TCMSpotLight::SetOuterCutOff( const float& degrees ) const
		{
			m_data->m_outerCutOff = degrees;
			UpdateData();
		}
	}
}
