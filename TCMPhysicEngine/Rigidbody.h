#pragma once
#include "TCMPhysicEngine/PhysicEngineDLL.h"
#include "MathLib/Vec3.h"
#include "MathLib/Quat.h"

#include <cstdint>

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			struct Collider;

			enum class TCMPHYSICENGINE_DLL EConstraints : uint8_t
			{
				FREEZE_X = 0,
				FREEZE_Y = 1,
				FREEZE_Z = 2,
				NONE
			};

			class TCMPHYSICENGINE_DLL Rigidbody
			{

			public:
				static Rigidbody * Instantiate();

				virtual ~Rigidbody() = default;

#pragma region Common Functions
				virtual void SetPosition(const Math::Vec3& position) = 0;
				virtual void SetRotation(const Math::Quat& rotation) = 0;
				void SetEulerAngles(const Math::Vec3& eulerAngles);
				virtual void SetVelocity(const Math::Vec3& velocity) = 0;

				virtual Math::Vec3 GetPosition() const = 0;
				virtual Math::Quat GetRotation() const = 0;
				Math::Vec3 GetEulerAngles() const;
				virtual Math::Vec3 GetVelocity() const = 0;

				void SetPositionConstraints(const EConstraints constraints);
				void SetRotationConstraints(const EConstraints constraints);

				EConstraints GetPositionConstraints() const;
				EConstraints GetRotationConstraints() const;

				void SetCenterOfMass(const Math::Vec3& center);
				Math::Vec3 GetCenterOfMass() const;

				void SetMass(const uint32_t& mass);
				uint32_t GetMass() const;

				void EnableCollisionDetection(const bool enabled);
				bool IsDetectingCollisions() const;

				void SetKinematic(const bool enabled);
				bool IsKinematic() const;

				void SetCollider(Collider * collider);
				Collider * GetCollider() const;

#pragma endregion 
			protected:

				Rigidbody() = default;

				virtual void ApplyCenterOfMass() = 0;
				virtual void ApplyMass() = 0;
				virtual void ApplyCollisionDetection() = 0;
				virtual void ApplyKinematic() = 0;
				virtual void ApplyCollider() = 0;

				EConstraints PositionConstraints = EConstraints::NONE;
				EConstraints RotationConstraints = EConstraints::NONE;

				uint32_t Mass = 10;
				Math::Vec3 CenterOfMass;

				bool DetectCollisions = true;
				bool UseGravity = true;
				bool Kinematic = false;

				Collider * BodyCollider = nullptr;
			};

		}
	}
}

