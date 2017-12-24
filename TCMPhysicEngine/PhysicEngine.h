#pragma once
#include "TCMPhysicEngine/PhysicEngineDLL.h"
#include "MathLib/Vec3.h"
#include <set>

namespace TCM
{
	namespace Engine
	{

		namespace Physics
		{
			enum CollisionShape : unsigned char;

			enum class EAxis : uint8_t
			{
				X_Axis = 0,
				Y_Axis = 1,
				Z_Axis = 2,
				None = 3
			};

			struct Rigidbody;
			struct Collider;

			struct TCMPHYSICENGINE_DLL Ray
			{
				Math::Vec3 Origin;
				Math::Vec3 Direction;
			};


			struct TCMPHYSICENGINE_DLL RaycastHit
			{
				bool Hit;
				Math::Vec3 Position;
				Math::Vec3 Normal;

				Rigidbody * HitRigidbody;
				Collider * HitCollider;

				float Distance;
			};


			class PhysicEngine abstract
			{
			public:
				virtual ~PhysicEngine();

				/**
				* \brief Initialize physic engine
				*/
				static TCMPHYSICENGINE_DLL void Init();

				/**
				* \brief Shutdowns physic engine
				*/
				static TCMPHYSICENGINE_DLL void Shutdown();

				static TCMPHYSICENGINE_DLL bool RegisterRigidbody(Rigidbody * rigidbody);

				static TCMPHYSICENGINE_DLL bool UnregisterRigidbody(Rigidbody * rigidbody);

				static TCMPHYSICENGINE_DLL void SetGravity(const Math::Vec3& gravity);

				static TCMPHYSICENGINE_DLL Math::Vec3 GetGravity();

				static TCMPHYSICENGINE_DLL void FixedUpdate(const float fixedDeltaTime);

				static TCMPHYSICENGINE_DLL RaycastHit Raycast(const Ray& ray);

				static void * GenColliderData();

				static void * GenRigidbodyData(const CollisionShape shape);

			protected:

				virtual void CoreInit() = 0;
				virtual void CoreShutdown() = 0;

				virtual bool CoreRegisterRigidbody(Rigidbody * rigidbody) = 0;
				virtual bool CoreUnregisterRigidbody(Rigidbody * rigidbody) = 0;

				virtual void CoreSetGravity(const Math::Vec3& gravity) = 0;

				virtual void CoreFixedUpdate(const float fixedDeltaTime) = 0;

				virtual RaycastHit CoreRaycast(const Ray& ray) const = 0;

				virtual void * CoreGenColliderData() = 0;

				virtual void * CoreGenRigidbodyData(const CollisionShape shape) = 0;

			private:
				/**
				* \brief Get the unique instance of the PhysicEngine
				* \return A reference to the PhysicEngine
				*/
				static PhysicEngine& GetInstance();

			protected:
				Math::Vec3 m_gravity;

				std::set<Rigidbody *> m_rigidbodyList;
			};
		}
	}
}
