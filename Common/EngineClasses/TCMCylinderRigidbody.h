#pragma once
#include "TCMComponentRigidBody.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief \n Component for rigid body, allowing collision to be done. \n
		*/
		class ENGINECLASSES_DLL_EXPORT TCMCylinderRigidbody : public TCMComponentRigidBody
		{
			TCM_CLASS( TCMCylinderRigidbody, TCMComponentRigidBody );

		public:
			TCMCylinderRigidbody();
		};
	}
}
