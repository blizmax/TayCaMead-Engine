#pragma once
#include "TCMAttenuatedLight.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief \n Component for point light. \n
		 */
		class ENGINECLASSES_DLL_EXPORT TCMPointLight : public TCMAttenuatedLight
		{
			TCM_CLASS(TCMPointLight, TCMAttenuatedLight);

		public:
			TCMPointLight();

			/**
			 * \brief \n Returns the direction of the point light. \n
			 * \return The direction of the point light.
			 */
			Math::Vec4 GetDirection() const override;
		};
	}
}
