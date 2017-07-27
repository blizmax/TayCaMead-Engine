#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletCollisionShape.h"
#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/btBulletCollisionCommon.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			namespace Bullet
			{
				BulletAPICollisionShape::~BulletAPICollisionShape()
				{
					delete m_bulletCollisionShape;
				}

				BulletCollisionShape* BulletAPICollisionShape::GetBulletCollisionShape() const
				{
					return m_bulletCollisionShape;
				}

				void BulletAPICollisionShape::SetBulletCollisionShape( BulletCollisionShape* shape )
				{
					m_bulletCollisionShape = shape;
				}
			}
		}
	}
}
