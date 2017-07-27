#pragma once
#include "TCMLightSource.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief Component for Directionnal light (like the sun).
		 */
		class ENGINECLASSES_DLL_EXPORT TCMDirectionnalLight : public TCMLightSource
		{
			TCM_CLASS(TCMDirectionnalLight, TCMLightSource);

		public:
			TCMDirectionnalLight();

			void TCMPoolCreate() override;

			/**
			 * \brief \n Gets the direction of the light. \n
			 * \return The direction of the light.
			 */
			Math::Vec4 GetDirection() const override;
		};
	}
}
