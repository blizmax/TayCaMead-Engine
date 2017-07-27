#pragma once
#include "TCMPhysicEngine/PhysicEngine/CollisionShape.h"

class btCollisionShape;
typedef btCollisionShape BulletCollisionShape;

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			namespace Bullet
			{
				struct BulletAPICollisionShape : CollisionShape
				{
					BulletAPICollisionShape() = default;

					explicit BulletAPICollisionShape( CollisionShapeData* info )
						: CollisionShape( info )
						, m_bulletCollisionShape( nullptr )
					{
					}

					~BulletAPICollisionShape();

					BulletCollisionShape* GetBulletCollisionShape() const;
					void SetBulletCollisionShape( BulletCollisionShape* shape );

				private:
					// * Collision shape defined with Bullet
					BulletCollisionShape* m_bulletCollisionShape;
				};
			}
		}
	}
}
