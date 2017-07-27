#pragma once
#include "TCMComponentRigidBody.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief \n Component for rigid body, allowing collision to be done. \n
		*/
		class ENGINECLASSES_DLL_EXPORT TCMBoxRigidbody : public TCMComponentRigidBody
		{
			TCM_CLASS( TCMBoxRigidbody, TCMComponentRigidBody );

			TCM_SERIALIZE_HIDDEN( m_size );

		public:
			TCMBoxRigidbody();
			void UpdateMesh();

			Math::Vec3 GetSize() const;
			void SetSize( const Math::Vec3& size );

			void Init() override;

		protected:
			Math::Vec3 m_size = { 1.0f, 1.0f, 1.0f };
		};
	}
}
