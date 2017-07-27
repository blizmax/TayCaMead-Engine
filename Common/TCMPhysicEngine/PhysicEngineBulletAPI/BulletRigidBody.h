#pragma once
#include "TCMPhysicEngine/PhysicEngine/RigidBody.h"
#include "BulletCollisionShape.h"

class btRigidBody;
typedef btRigidBody BulletRigidBody;

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			namespace Bullet
			{
				struct BulletAPIRigidBody : RigidBody
				{
					BulletAPIRigidBody() = default;

					BulletAPIRigidBody( RigidBodyData* infoR
					                    , CollisionShapeData* infoC )
						: RigidBody( infoR )
						, m_bulletRigidBody( nullptr )
					{
						m_collisionShape = new BulletAPICollisionShape( infoC );
					}

					~BulletAPIRigidBody();

					Math::Vec3 GetPosition() const override;
					void SetPosition( Math::Vec3 position ) override;

					void SetRotation( const Math::Quat& rotation ) override;

					void SetEulerAngles( const Math::Vec3& angles ) override;

					float GetFriction() const override;
					void SetFriction( const float& friction ) override;

					float GetRestitution() const override;
					void SetRestitution( const float& restitution ) override;

					void Update() override;

					void SetBulletRigidBody( BulletRigidBody* rigidBody );
					BulletRigidBody* GetBulletRigidBody() const;

				protected:
					BulletRigidBody* m_bulletRigidBody;
				};
			}
		}
	}
}
