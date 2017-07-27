#pragma once
#include "TCMAttenuatedLight.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief \n Component for spot light. \n
		 */
		class ENGINECLASSES_DLL_EXPORT TCMSpotLight : public TCMAttenuatedLight
		{
			TCM_CLASS(TCMSpotLight, TCMAttenuatedLight);

			//TCM_SERIALIZE( m_data );

			TCM_SERIALIZE( m_data->m_cutOff,
				m_data->m_outerCutOff );

			TCMSpotLight();
			/**
			 * \brief \n Gets the direction of the spot light. \n
			 * \return The direction of the spot light.
			 */
			Math::Vec4 GetDirection() const override;

			/**
			 * \brief \n Sets the cutoff parameters of the spot light. \n
			 * \param cutOff _IN_		Cutoff value of the spot light. (degrees)
			 * \param outerCutOff _IN_		Outer Cutoff value of the spot light. (degrees)
			 */
			void SetSpotLightValues( const float& cutOff, const float& outerCutOff ) const;
			/**
			 * \brief \n Sets the cutoff of the spot light. \n
			 * \param degrees _IN_	 Cutoff value of the spot light.
			 */
			void SetCutOff( const float& degrees ) const;
			/**
			 * \brief \n Sets the outer cutoff of the spot light. \n
			 * \param degrees _IN_	 Outer cutoff value of the spot light.
			 */
			void SetOuterCutOff( const float& degrees ) const;
		};
	}
}
