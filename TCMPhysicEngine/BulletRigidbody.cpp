#include "BulletRigidbody.h"
#include <TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/BulletDynamics/Dynamics/btRigidBody.h>
#include <TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/LinearMath/btTransform.h>
#include <TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/LinearMath/btMotionState.h>

#include "TCMPhysicEngine/PhysicUtils.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{

			BulletRigidbody::BulletRigidbody()
			{
			}


			BulletRigidbody::~BulletRigidbody()
			{
			}

			void BulletRigidbody::SetPosition(const Math::Vec3& position)
			{
				if (!IsKinematic())
				{
					btTransform transfrom = bulletRigidbody->getCenterOfMassTransform();
					transfrom.setOrigin(TobtVec3(position));
					bulletRigidbody->setCenterOfMassTransform(transfrom);
				}
				else
				{
					btTransform transfrom;
					bulletRigidbody->getMotionState()->getWorldTransform(transfrom);
					transfrom.setOrigin(TobtVec3(position));
					bulletRigidbody->getMotionState()->setWorldTransform(transfrom);
				}
			}

			void BulletRigidbody::SetRotation(const Math::Quat& rotation)
			{
			}

			void BulletRigidbody::SetVelocity(const Math::Vec3& velocity)
			{
			}

			Math::Vec3 BulletRigidbody::GetPosition() const
			{
				btTransform transfrom = bulletRigidbody->getCenterOfMassTransform();
				Math::Vec3 origin = ToVec3(transfrom.getOrigin());
				return origin;
			}

			Math::Quat BulletRigidbody::GetRotation() const
			{
				btTransform trans = bulletRigidbody->getCenterOfMassTransform();
				return ToQuat(trans.getRotation());
			}

			Math::Vec3 BulletRigidbody::GetVelocity() const
			{
				return ToVec3(bulletRigidbody->getLinearVelocity());
			}

			void BulletRigidbody::ApplyPositionConstraints()
			{
			}

			void BulletRigidbody::ApplyRotationConstraints()
			{
			}

			void BulletRigidbody::ApplyCenterOfMass()
			{
			}

			void BulletRigidbody::ApplyMass()
			{
			}

			void BulletRigidbody::ApplyCollisionDetection()
			{
			}

			void BulletRigidbody::ApplyKinematic()
			{
			}

			void BulletRigidbody::ApplyCollider()
			{
			}
		}
	}
}
