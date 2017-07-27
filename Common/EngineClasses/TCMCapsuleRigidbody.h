#pragma once
#include "TCMComponentRigidBody.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief \n Component for rigid body, allowing collision to be done. \n
		*/
		class ENGINECLASSES_DLL_EXPORT TCMCapsuleRigidbody : public TCMComponentRigidBody
		{
			TCM_CLASS( TCMCapsuleRigidbody, TCMComponentRigidBody );

			TCM_SERIALIZE_HIDDEN( m_radius, m_height );

		public:
			TCMCapsuleRigidbody();

			float GetRadius() const;
			void SetRadius( const float radius );
			float GetHeight() const;
			void SetHeight( const float height );

			void Init() override;

		protected:
			void UpdateMesh();

			float m_radius = 0.5f;
			float m_height = 1.0f;
		};
	}
}
