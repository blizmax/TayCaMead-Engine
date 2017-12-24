#include "PhysicEngine.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			void PhysicEngine::Init()
			{
				GetInstance().CoreInit();
			}

			void PhysicEngine::Shutdown()
			{
				GetInstance().CoreShutdown();
			}
			bool PhysicEngine::RegisterRigidbody(Rigidbody * rigidbody)
			{
				std::set<Rigidbody *>& temp_set = GetInstance().m_rigidbodyList;
				if (temp_set.find(rigidbody) != temp_set.end())
					return false;
				temp_set.insert(rigidbody);

				return GetInstance().CoreRegisterRigidbody(rigidbody);
			}
			bool PhysicEngine::UnregisterRigidbody(Rigidbody * rigidbody)
			{

				std::set<Rigidbody *>& temp_set = GetInstance().m_rigidbodyList;
				if (temp_set.find(rigidbody) == temp_set.end())
					return false;

				const bool result = GetInstance().CoreUnregisterRigidbody(rigidbody);

				temp_set.erase(rigidbody);

				return result;
			}

			void PhysicEngine::SetGravity(const Math::Vec3& gravity)
			{
				GetInstance().m_gravity = gravity;
				GetInstance().CoreSetGravity(gravity);

			}

			Math::Vec3 PhysicEngine::GetGravity()
			{
				return GetInstance().m_gravity;
			}

			void PhysicEngine::FixedUpdate(const float fixedDeltaTime)
			{
				GetInstance().CoreFixedUpdate(fixedDeltaTime);
			}

			RaycastHit PhysicEngine::Raycast(const Ray& ray)
			{
				return GetInstance().CoreRaycast(ray);
			}
		}
	}
}