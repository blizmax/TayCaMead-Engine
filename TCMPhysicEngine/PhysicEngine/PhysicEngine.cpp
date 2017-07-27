#include <memory>
#include <mutex>

#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"

#if defined(TCM_BULLET_API)
#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletPhysicEngine.h"
typedef TCM::Engine::Physics::Bullet::BulletPhysicEngine TCMOverridedPhysicEngine;
#else
	static_assert(false && "NO PHYSIC ENGINE SET !");
#endif

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			void PhysicEngine::Initialize( const Math::Vec3& gravity )
			{
				GetInstance().CoreInitialize( gravity );
			}

			void PhysicEngine::Run( const float fixedDeltaTime )
			{
				GetInstance().CoreRun( fixedDeltaTime );
			}

			void PhysicEngine::Shutdown()
			{
				GetInstance().BaseShutdown();
			}

			void PhysicEngine::SetGravity( const Math::Vec3& gravity )
			{
				GetInstance().m_gravity = gravity;
				GetInstance().CoreSetGravity( gravity );
			}

			const Math::Vec3& PhysicEngine::GetGravity()
			{
				return GetInstance().CoreGetGravity();
			}

			CollisionShape* PhysicEngine::CreateCollisionShape( CollisionShapeData* shapeInfo )
			{
				return GetInstance().CoreCreateCollisionShape( shapeInfo );
			}

			void PhysicEngine::DeleteCollisionShape( CollisionShape* shape )
			{
				GetInstance().CoreDeleteCollisionShape( shape );
			}

			RigidBody* PhysicEngine::CreateRigidBody( RigidBodyData* rigidBodyInfo, CollisionShapeData* shapeInfo )
			{
				return GetInstance().BaseCreateRigidBody( rigidBodyInfo, shapeInfo );
			}

			void PhysicEngine::DeleteRigidBody( RigidBody* rigidBody )
			{
				GetInstance().BaseDeleteRigidBody( rigidBody );
			}

			void PhysicEngine::DeactivateRigidBody( RigidBody* rigidBody )
			{
				GetInstance().BaseDeactivateRigidBody( rigidBody );
			}

			void PhysicEngine::ActivateRigidBody( RigidBody* rigidBody )
			{
				GetInstance().BaseActivateRigidBody( rigidBody );
			}

			const float& PhysicEngine::GetUnitConvertor()
			{
				return GetInstance().unitConvertor;
			}

			Math::Vec3 PhysicEngine::GetDefaultInertie()
			{
				return GetInstance().m_inertie;
			}

			float PhysicEngine::GetDefaultFriction()
			{
				return GetInstance().m_friction;
			}

			float PhysicEngine::GetDefaultRestitution()
			{
				return GetInstance().m_restitution;
			}

			void PhysicEngine::SetDefaultInertie( const Math::Vec3& vec )
			{
				GetInstance().m_inertie = vec;
			}

			void PhysicEngine::SetDefaultFriction( const float value )
			{
				GetInstance().m_friction = value;
			}

			void PhysicEngine::SetDefaultRestitution( const float value )
			{
				GetInstance().m_restitution = value;
			}

			PhysicEngine::PhysicEngine()
			{
				m_rigidBodyList = new std::vector<RigidBody*>;
			}

			void PhysicEngine::BaseShutdown()
			{
				if ( m_rigidBodyList )
				{
					for ( auto it = m_rigidBodyList->begin(); it != m_rigidBodyList->end(); ++it )
					{
						RigidBody* oneBody = *it;
						if ( oneBody )
						{
							CoreDeleteRigidBody( oneBody );
						}
					}

					delete m_rigidBodyList;
					m_rigidBodyList = nullptr;
				}
				CoreShutdown();
			}

			RigidBody* PhysicEngine::BaseCreateRigidBody( RigidBodyData* rigidBodyInfo, CollisionShapeData* shapeInfo )
			{
				RigidBody* result = CoreCreateRigidBody( rigidBodyInfo, shapeInfo );
				m_rigidBodyList->push_back( result );
				return result;
			}

			void PhysicEngine::BaseDeleteRigidBody( RigidBody* rigidBody )
			{
				if ( m_rigidBodyList )
				{
					auto it = m_rigidBodyList->begin();
					for ( ; it != m_rigidBodyList->end(); ++it )
						if ( *it == rigidBody )
							break;

					if ( it != m_rigidBodyList->end() )
					{
						CoreDeleteRigidBody( *it );
						m_rigidBodyList->erase( it );
					}
				}
				delete rigidBody;
			}

			void PhysicEngine::BaseDeactivateRigidBody( RigidBody* rigidBody )
			{
				auto it = m_rigidBodyList->begin();
				for ( ; it != m_rigidBodyList->end(); ++it )
					if ( *it == rigidBody )
						break;

				if ( it != m_rigidBodyList->end() )
				{
					CoreDeactivateRigidBody( rigidBody );
					m_rigidBodyList->erase( it );
				}
			}

			void PhysicEngine::BaseActivateRigidBody( RigidBody* rigidBody )
			{
				auto it = m_rigidBodyList->begin();
				for ( ; it != m_rigidBodyList->end(); ++it )
					if ( *it == rigidBody )
						break;
				if ( it == m_rigidBodyList->end() )
				{
					CoreActivateRigidBody( rigidBody );
					m_rigidBodyList->push_back( rigidBody );
				}
			}

			PhysicEngine& PhysicEngine::GetInstance()
			{
				static std::unique_ptr<PhysicEngine> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new TCMOverridedPhysicEngine() );
				                }
				);

				return *m_instance.get();
			}
		}
	}
}
