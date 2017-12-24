#include "Rigidbody.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			void Rigidbody::SetPosition(const Math::Vec3& position)
			{
				Position = position;
				ApplyPosition();
			}

			void Rigidbody::SetRotation(const Math::Vec3& rotation)
			{
				Rotation = rotation;
				ApplyRotation();
			}

			void Rigidbody::SetVelocity(const Math::Vec3& velocity)
			{
				Velocity = velocity;
				ApplyVelocity();
			}

			Math::Vec3 Rigidbody::GetPosition() const
			{
				return Velocity;
			}

			Math::Vec3 Rigidbody::GetRotation() const
			{
				return Rotation;
			}

			Math::Vec3 Rigidbody::GetVelocity() const
			{
				return Velocity;
			}

			void Rigidbody::SetPositionConstraints(const EConstraints constraints)
			{
				PositionConstraints = constraints;
				ApplyPositionConstraints();
			}

			void Rigidbody::SetRotationConstraints(const EConstraints constraints)
			{
				RotationConstraints = constraints;
				ApplyRotationConstraints();
			}

			EConstraints Rigidbody::GetPositionConstraints() const
			{
				return PositionConstraints;
			}

			EConstraints Rigidbody::GetRotationConstraints() const
			{
				return RotationConstraints;
			}

			void Rigidbody::SetCenterOfMass(const Math::Vec3& center)
			{
				if (CenterOfMass == center)
					return;

				CenterOfMass = center;
				ApplyCenterOfMass();
			}

			Math::Vec3 Rigidbody::GetCenterOfMass() const
			{
				return CenterOfMass;
			}

			void Rigidbody::SetMass(const uint32_t& mass)
			{
				if (Mass == mass)
					return;

				Mass = mass;
				ApplyMass();
			}

			uint32_t Rigidbody::GetMass() const
			{
				return Mass;
			}

			void Rigidbody::EnableCollisionDetection(const bool enabled)
			{
				if (DetectCollisions == enabled)
					return;
				DetectCollisions = enabled;
				ApplyCollisionDetection();
			}

			bool Rigidbody::IsDetectingCollisions() const
			{
				return DetectCollisions;
			}

			void Rigidbody::SetKinematic(const bool enabled)
			{
				if (Kinematic == enabled)
					return;
				Kinematic = enabled;
				ApplyKinematic();
			}

			bool Rigidbody::IsKinematic() const
			{
				return Kinematic;
			}

			void Rigidbody::SetCollider(Collider* collider)
			{
				if (BodyCollider == collider)
					return;
				BodyCollider = collider;
				ApplyCollider();
			}

			Collider* Rigidbody::GetCollider() const
			{
				return BodyCollider;
			}
		}
	}
}