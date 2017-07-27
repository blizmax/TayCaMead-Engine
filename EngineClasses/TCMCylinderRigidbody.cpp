#include "TCMCylinderRigidbody.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMCylinderRigidbody )

		TCMCylinderRigidbody::TCMCylinderRigidbody()
		{
			m_collisionShapeInfo = new Engine::Physics::CollisionShapeData( Engine::Physics::CYLINDER );
			InitRigidbody();
		}
	}
}
