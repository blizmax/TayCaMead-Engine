#pragma once
#include "TCMComponentRigidBody.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief \n Component for rigid body, allowing collision to be done. \n
		*/
		class ENGINECLASSES_DLL_EXPORT TCMSphereRigidbody : public TCMComponentRigidBody
		{
			TCM_CLASS( TCMSphereRigidbody, TCMComponentRigidBody );

			TCM_SERIALIZE_HIDDEN( m_radius )

		public:
			TCMSphereRigidbody();

			float GetRadius() const;
			void SetRadius( const float radius );

			void Init() override;

		protected:
			void UpdateMesh();
			float m_radius = 1.0f;
		};
	}
}
