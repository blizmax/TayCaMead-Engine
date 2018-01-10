#include "BulletCollider.h"
#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/btBulletCollisionCommon.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			void BulletBoxCollider::Reset()
			{
				if (data)
					delete static_cast<btBoxShape*>(data);
				data = new btBoxShape(btVector3(Extends.x, Extends.y, Extends.z));

			}

			void BulletSphereCollider::Reset()
			{
				if (data)
					delete static_cast<btSphereShape*>(data);
				data = new btSphereShape(Radius);
			}

			void BulletPlaneCollider::Reset()
			{
				if (data)
					delete static_cast<btStaticPlaneShape *>(data);
				data = new btStaticPlaneShape(btVector3(Normal.x, Normal.y, Normal.z), Constant);
			}

			void BulletCylinderCollider::Reset()
			{
				if (data)
					delete static_cast<btCylinderShape*>(data);
				data = new btCylinderShape(btVector3(Radius, Height, Radius));
			}

			void BulletCapsuleCollider::Reset()
			{
				if (data)
					delete static_cast<btCapsuleShape*>(data);
				data = new btCapsuleShape(Radius, Height);
			}
		}
	}
}