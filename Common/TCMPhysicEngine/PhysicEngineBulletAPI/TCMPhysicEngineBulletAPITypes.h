#pragma once

#include "TCMPhysicEngine/PhysicEngine/TCMPhysicEngineTypes.h"
#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/btBulletDynamicsCommon.h"
#include "TCMPhysicEngine/PhysicEngine/RigidBody.h"

#ifdef _DEBUG
#pragma comment(lib, "TCMPhysicEngine/debug/BulletDynamics.lib")
#pragma comment(lib, "TCMPhysicEngine/debug/BulletCollision.lib")
#pragma comment(lib, "TCMPhysicEngine/debug/LinearMath.lib")
#else
	#pragma comment(lib, "TCMPhysicEngine/release/BulletDynamics.lib")
	#pragma comment(lib, "TCMPhysicEngine/release/BulletCollision.lib")
	#pragma comment(lib, "TCMPhysicEngine/release/LinearMath.lib")
#endif

#define TobtVector3(vector) btVector3(vector.x, vector.y, vector.z)
#define TobtQuaternion(quat) btQuaternion(quat.x, quat.y, quat.z, quat.w)

#define ToEngineVector3(vector) TCM::Engine::Math::Vec3(vector.x(), vector.y(), vector.z())
#define ToEngineQuaternion(quat) TCM::Engine::Math::Quat(quat.x(), quat.y(), quat.z(), quat.w())

class btBroadphaseInterface;
typedef btBroadphaseInterface BulletBroadphase;

class btDefaultCollisionConfiguration;
typedef btDefaultCollisionConfiguration BulletCollisionConfig;

class btCollisionDispatcher;
typedef btCollisionDispatcher BulletDispatcher;

class btSequentialImpulseConstraintSolver;
typedef btSequentialImpulseConstraintSolver BulletSolver;

class btDiscreteDynamicsWorld;
typedef btDiscreteDynamicsWorld BulletDynamicWorld;

class btRigidBody;
typedef btRigidBody BulletRigidBody;

class btCollisionShape;
typedef btCollisionShape BulletCollisionShape;

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			namespace Bullet
			{
#pragma region typedefs

#pragma endregion


				// * Rigid Body defined with the Bullet API
				// * Bullet's Collision shape and rigid body need to be created after creating this struct
				
			}
		}
	}
}
