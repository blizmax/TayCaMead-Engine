#pragma once
#include "TCMPhysicEngine/PhysicEngineDLL.h"

#include "MathLib/Vec3.h"
#include "MathLib/Vec2.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			enum CollisionShape : uint8_t
			{
				BOX, SPHERE, PLANE, CAPSULE, CYLINDER, None
			};

			enum class EAxis : unsigned char;
			class Rigidbody;

			class TCMPHYSICENGINE_DLL Collider abstract
			{
			public:
				virtual ~Collider() = default;
				Collider();

				virtual void Reset() = 0;

				void * GetData() const;
				void AttachRigidbody(Rigidbody * AttachedRigidbody);
				Rigidbody * GetRigidbody() const;

				void SetTrigger(const bool trigger);
				bool GetTrigger() const;

				void SetCenter(const Math::Vec3& center);
				Math::Vec3 GetCenter() const;

			protected:
				Rigidbody * AttachedRigidbody;

				bool IsTrigger = false;
				Math::Vec3 Center;

				void * data = nullptr;
			};

			class TCMPHYSICENGINE_DLL BoxCollider : public Collider
			{
			public:
				BoxCollider();

				void SetExtends(const Math::Vec3& extends);
				Math::Vec3 GetExtends() const;

			protected:
				Math::Vec3 Extends;
			};

			class TCMPHYSICENGINE_DLL SphereCollider : public Collider
			{
			public:
				SphereCollider();

				void SetRadius(const float radius);
				float GetRadius() const;

			protected:
				float Radius;
			};

			class TCMPHYSICENGINE_DLL PlaneCollider : public Collider
			{
			public:
				PlaneCollider();

				void SetNormal(const Math::Vec3& normal);
				Math::Vec3 GetNormal() const;

				void SetConstant(const float constant);
				float GetConstant() const;

			protected:
				Math::Vec3 Normal;
				float Constant;
			};

			class TCMPHYSICENGINE_DLL CylinderCollider : public Collider
			{
				CylinderCollider();

				void SetHeight(const float height);
				float GetHeight() const;

				void SetRadius(const float radius);
				float GetRadius() const;

			protected:
				float Height;
				float Radius;
			};

			class TCMPHYSICENGINE_DLL CapsuleCollider : public Collider
			{
			public:
				CapsuleCollider();

				void SetHeight(const float height);
				float GetHeight() const;

				void SetRadius(const float radius);
				float GetRadius() const;

				void SetAxis(const EAxis Direction);
				EAxis GetAxis() const;

			protected:

				float Height;
				float Radius;

				EAxis Direction;
			};
		}
	}
}