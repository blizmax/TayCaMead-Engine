#include "Collider.h"
#include "Rigidbody.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			void* Collider::GetData() const
			{
				return data;
			}

			void Collider::AttachRigidbody(Rigidbody* AttachedRigidbody)
			{
				//TODO: Complete with abstraction
				if (AttachedRigidbody != this->AttachedRigidbody)
				{
					this->AttachedRigidbody = AttachedRigidbody;
					AttachedRigidbody->SetCollider(this);
				}
			}

			Rigidbody* Collider::GetRigidbody() const
			{
				return AttachedRigidbody;
			}

			void Collider::SetTrigger(const bool trigger)
			{
				//TODO: Complete with abstraction
				IsTrigger = trigger;
			}

			bool Collider::GetTrigger() const
			{
				return IsTrigger;
			}

			void Collider::SetCenter(const Math::Vec3& center)
			{
				Center = center;
			}

			Math::Vec3 Collider::GetCenter() const
			{
				return Center;
			}

			void BoxCollider::SetExtends(const Math::Vec3& extends)
			{
				Extends = extends;
				Reset();
			}

			Math::Vec3 BoxCollider::GetExtends() const
			{
				return Extends;
			}

			void SphereCollider::SetRadius(const float radius)
			{
				Radius = radius;
				Reset();
			}

			float SphereCollider::GetRadius() const
			{
				return Radius;
			}

			void PlaneCollider::SetNormal(const Math::Vec3& normal)
			{
				Normal = normal;
				Reset();
			}

			Math::Vec3 PlaneCollider::GetNormal() const
			{
				return Normal;
			}

			void PlaneCollider::SetConstant(const float constant)
			{
				Constant = constant;
			}

			float PlaneCollider::GetConstant() const
			{
				return Constant;
			}

			void CylinderCollider::SetHeight(const float height)
			{
				Height = height;
				Reset();
			}

			float CylinderCollider::GetHeight() const
			{
				return Height;
			}

			void CylinderCollider::SetRadius(const float radius)
			{
				Radius = radius;
				Reset();
			}

			float CylinderCollider::GetRadius() const
			{
				return Radius;
			}

			void CapsuleCollider::SetHeight(const float height)
			{
				Height = height;
				Reset();
			}

			float CapsuleCollider::GetHeight() const
			{
				return Height;
			}

			void CapsuleCollider::SetRadius(const float radius)
			{
				Radius = radius;
				Reset();
			}

			float CapsuleCollider::GetRadius() const
			{
				return Radius;
			}

			void CapsuleCollider::SetAxis(const EAxis Direction)
			{
				this->Direction = Direction;
			}

			EAxis CapsuleCollider::GetAxis() const
			{
				return Direction;
			}
		}
	}
}