#pragma once
#include "Rigidbody.h"

class btRigidBody;

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			class BulletRigidbody : Rigidbody
			{
			public:
				BulletRigidbody();
				~BulletRigidbody();

				virtual void SetPosition(const Math::Vec3& position) override;
				virtual void SetRotation(const Math::Quat& rotation) override;
				virtual void SetVelocity(const Math::Vec3& velocity) override;

				virtual Math::Vec3 GetPosition() const override;
				virtual Math::Quat GetRotation() const override;
				virtual Math::Vec3 GetVelocity() const override;

			protected:
				virtual void ApplyPositionConstraints() override;
				virtual void ApplyRotationConstraints() override;
				virtual void ApplyCenterOfMass() override;
				virtual void ApplyMass() override;
				virtual void ApplyCollisionDetection() override;
				virtual void ApplyKinematic() override;
				virtual void ApplyCollider() override;

			public:

			protected:
				btRigidBody * bulletRigidbody;
			};

		}
	}
}
