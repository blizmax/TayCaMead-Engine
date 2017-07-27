#pragma once
#include "MathLib/Vec3.h"
#include <vector>

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			struct RigidBody;

			struct CollisionData
			{
				bool m_enteredInCollision = false;
				RigidBody* m_collisionedBody = nullptr;
				std::vector<Math::Vec3> m_hitPoints;
				std::vector<Math::Vec3> m_normals;

				void Reset()
				{
					m_enteredInCollision = false;
					m_collisionedBody = nullptr;
					m_hitPoints.clear();
					m_normals.clear();
				}
			};
		}
	}
}
