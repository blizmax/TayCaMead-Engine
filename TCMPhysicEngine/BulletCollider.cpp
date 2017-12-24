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
					delete data;
				data = new btBoxShape(btVector3(Extends.x, Extends.y, Extends.z));

			}

			void BulletSphereCollider::Reset()
			{
				if (data)
					delete data;
				data = new btSphereShape(Radius);
			}

			void BulletPlaneCollider::Reset()
			{
				if (data)
					delete data;
				data = new btBoxShape(btVector3(Extends.x, Extends.y, Extends.z));
			}

			void BulletCylinderCollider::Reset()
			{
				if (data)
					delete data;
				data = new btCylinderShape(btVector3(Extends.x, Extends.y, Extends.z));
			}

			void BulletCapsuleCollider::Reset()
			{
				if (data)
					delete data;
				data = new btBoxShape(btVector3(Extends.x, Extends.y, Extends.z));
			}
		}
	}
}