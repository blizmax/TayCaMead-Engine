#pragma once
#include "Rigidbody.h"

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

				virtual Math::Vec3 GetPosition() const override;
				virtual Math::Vec3 GetRotation() const override;
				virtual Math::Vec3 GetVelocity() const override;

			protected:
				virtual void ApplyPosition() override;
				virtual void ApplyRotation() override;
				virtual void ApplyVelocity() override;
				virtual void ApplyPositionConstraints() override;
				virtual void ApplyRotationConstraints() override;
				virtual void ApplyCenterOfMass() override;
				virtual void ApplyMass() override;
				virtual void ApplyCollisionDetection() override;
				virtual void ApplyKinematic() override;
				virtual void ApplyCollider() override;
			};

		}
	}
}
