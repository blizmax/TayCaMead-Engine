#pragma once

#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "BulletRigidBody.h"

#ifdef _DEBUG
#pragma comment(lib, "TCMPhysicEngine/debug/BulletDynamics.lib")
#pragma comment(lib, "TCMPhysicEngine/debug/BulletCollision.lib")
#pragma comment(lib, "TCMPhysicEngine/debug/LinearMath.lib")
#else
	#pragma comment(lib, "TCMPhysicEngine/release/BulletDynamics.lib")
	#pragma comment(lib, "TCMPhysicEngine/release/BulletCollision.lib")
	#pragma comment(lib, "TCMPhysicEngine/release/LinearMath.lib")
#endif

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

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			namespace Bullet
			{
				class BulletPhysicEngine final : public PhysicEngine
				{
				public:
					BulletPhysicEngine() = default;
					~BulletPhysicEngine();

				private:

					void CoreInitialize( const Math::Vec3& gravity ) override;
					void CoreRun( const float fixedDeltaTime ) override;
					void CoreShutdown() override;

					BulletRigidBody* CreateBulletRigidBody( CollisionShapeData* infoC, RigidBodyData* infoR ) const;
					static BulletCollisionShape* CreateBulletCollisionShape( CollisionShapesType shape );

					CollisionShape* CoreCreateCollisionShape( CollisionShapeData* shapeInfo ) override;
					void CoreDeleteCollisionShape( CollisionShape* shape ) override;

					RigidBody* CoreCreateRigidBody( RigidBodyData* rigidBodyInfo, CollisionShapeData* shapeInfo ) override;
					void CoreDeleteRigidBody( RigidBody* rigidBody ) override;

					void CoreSetGravity( const Math::Vec3& gravity ) override;

					void CoreDeactivateRigidBody( RigidBody* rigidBody ) override;
					void CoreActivateRigidBody( RigidBody* rigidBody ) override;

					BulletBroadphase* m_broadphase;
					BulletDispatcher* m_dispatcher;
					BulletSolver* m_solver;
					BulletCollisionConfig* m_collisionConfiguration;
					BulletDynamicWorld* m_dynamicWorld;
				};
			}
		}
	}
}
