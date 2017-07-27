#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletPhysicEngine.h"
#include "TCMLogger/Logger.h"
#include "TCMInputManager/InputManager.h"
#include "TCMTime/Time.h"
#include "TCMPhysicEngine/PhysicUtils.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			namespace Bullet
			{
				BulletPhysicEngine::~BulletPhysicEngine()
				{
					CoreShutdown();
				}

				void BulletPhysicEngine::CoreInitialize( const Math::Vec3& gravity )
				{
					TCMDEBUG("Initializing Physic Engine...");

					m_broadphase = new btDbvtBroadphase();
					m_collisionConfiguration = new BulletCollisionConfig();
					m_dispatcher = new BulletDispatcher( m_collisionConfiguration );
					m_solver = new BulletSolver();

					m_dynamicWorld = new BulletDynamicWorld( m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration );
					m_dynamicWorld->setGravity( TobtVector3(gravity) );
					m_gravity = gravity;

					unitConvertor = 0.5f;
				}

				void BulletPhysicEngine::CoreRun( const float fixedDeltaTime )
				{
					//std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

					for ( auto it = m_rigidBodyList->begin(); it != m_rigidBodyList->end(); ++it )
					{
						BulletAPIRigidBody* rigidbody = dynamic_cast<BulletAPIRigidBody *>(*it);
						BulletRigidBody* body = rigidbody->GetBulletRigidBody();
						RigidBodyData* bodyInfo = rigidbody->GetRigidBodyInfo();
						CollisionShapeData* collisionShapeInfo = rigidbody->GetCollisionShape()->GetCollisionShapeInfo();
						CollisionData* collisionData = rigidbody->m_collisionData;

						// =-=-= RESET COLLISION DATA FOR POTENTIAL COLLISION =-=-= //
						if ( collisionData )
							collisionData->Reset();

						// =-=-= GRAVITY CHECK =-=-= //
						if ( !bodyInfo->m_useGravity )
							body->setGravity( btVector3( 0.f, 0.f, 0.f ) );
						else if ( body->getGravity() != TobtVector3(m_gravity) )
							body->setGravity( TobtVector3(m_gravity) );

						// =-=-= OBJECT BECAME KINEMATIC =-=-= //
						if ( bodyInfo->m_isKinematicChanged )
						{
							if ( bodyInfo->m_isKinematic )
								body->setCollisionFlags( body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
							else
								body->setCollisionFlags( body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT );
							bodyInfo->m_isKinematicChanged = false;
						}

						// =-=-= OBJECTS BECAME RESTRAINED =-=-= //
						if ( bodyInfo->m_frozenLibertiesChanged )
						{
							AxisLiberties* liberties = &bodyInfo->m_frozenLiberties;
							btVector3 lFactor = btVector3( liberties->m_posX, liberties->m_posY, liberties->m_posZ );
							body->setLinearFactor( lFactor );

							btVector3 aFactor = btVector3( liberties->m_rotX, liberties->m_rotY, liberties->m_rotZ );
							body->setAngularFactor( aFactor );

							bodyInfo->m_frozenLibertiesChanged = false;
						}

						// =-=-= OBJECTS CHANGED INERTIA / MASS =-=-= //
						if ( bodyInfo->m_inertiaChanged || bodyInfo->m_massChanged )
						{
							btVector3 inertia = TobtVector3(bodyInfo->m_inertia);
							float mass = bodyInfo->m_mass;

							m_dynamicWorld->removeRigidBody( body );
							body->getCollisionShape()->calculateLocalInertia( mass, inertia );
							body->setMassProps( mass, inertia );
							m_dynamicWorld->addRigidBody( body );

							bodyInfo->m_inertiaChanged = bodyInfo->m_massChanged = false;
						}

						// =-=-= OBJECTS CHANGED SCALE =-=-= //
						if ( collisionShapeInfo->m_scaleChanged )
						{
							body->getCollisionShape()->setLocalScaling( TobtVector3(collisionShapeInfo->m_scale) );
							collisionShapeInfo->m_scaleChanged = false;
						}
					}
#
					// =-=-= BULLET SIMULATION =-=-= //
					m_dynamicWorld->stepSimulation( fixedDeltaTime
					                                , 0 );

					// =-=-= CHECK COLLISION =-=-= //
					int numberOfCollision = m_dynamicWorld->getDispatcher()->getNumManifolds();
					for ( int idx = 0; idx < numberOfCollision; ++idx )
					{
						btPersistentManifold* persistentManifold = m_dynamicWorld->getDispatcher()->getManifoldByIndexInternal( idx );

						int numContact = persistentManifold->getNumContacts();

						// =-=-= ITERATE ON EVERY REGISTERED CONTACT =-=-= //
						for ( int currContact = 0; currContact < numContact; ++currContact )
						{
							btManifoldPoint& point = persistentManifold->getContactPoint( currContact );
							const btCollisionObject* first = persistentManifold->getBody0();
							const btCollisionObject* sec = persistentManifold->getBody1();

							auto firstRigidBody = static_cast<RigidBody*>(first->getUserPointer())->m_collisionData;
							auto secondRigidBody = static_cast<RigidBody*>(sec->getUserPointer())->m_collisionData;

							firstRigidBody->m_enteredInCollision = true;
							firstRigidBody->m_collisionedBody = static_cast<RigidBody*>(sec->getUserPointer());

							secondRigidBody->m_enteredInCollision = true;
							secondRigidBody->m_collisionedBody = static_cast<RigidBody*>(first->getUserPointer());

							firstRigidBody->m_hitPoints.push_back( ToEngineVector3(point.getPositionWorldOnA()) );
							secondRigidBody->m_hitPoints.push_back( ToEngineVector3(point.getPositionWorldOnB()) );
							secondRigidBody->m_normals.push_back( ToEngineVector3(point.m_normalWorldOnB) );
						}
					}

					/*std::chrono::high_resolution_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();

					std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(timeEnd - start);

					TCMINFO("Physic engine loop: " + std::to_string(duration.count()) + " seconds.");*/
				}

				void BulletPhysicEngine::CoreShutdown()
				{
					TCMDEBUG("Destroying Physic Engine with Bullet API...");

					if ( m_broadphase )
					{
						delete m_broadphase;
						m_broadphase = nullptr;
					}

					if ( m_dispatcher )
					{
						delete m_dispatcher;
						m_dispatcher = nullptr;
					}

					if ( m_solver )
					{
						delete m_solver;
						m_solver = nullptr;
					}

					if ( m_collisionConfiguration )
					{
						delete m_collisionConfiguration;
						m_collisionConfiguration = nullptr;
					}

					if ( m_dynamicWorld )
					{
						delete m_dynamicWorld;
						m_dynamicWorld = nullptr;
					}
				}

				BulletCollisionShape* BulletPhysicEngine::CreateBulletCollisionShape( CollisionShapesType shape )
				{
					switch ( shape )
					{
						case SPHERE:
							return new btSphereShape( 1.f );
						case PLANE:
							return new btStaticPlaneShape( btVector3( 0.f, 1.f, 0.f ), 1.f );
						case CAPSULE:
							return new btCapsuleShape( 0.5f, 1.f );
						case BOX:
							return new btBoxShape( btVector3( 1.f, 1.f, 1.f ) );
						case CYLINDER:
							return new btCylinderShape( btVector3( 0.5f, 1.f, 0.5f ) );
						default:
							return new btEmptyShape();
					}
				}

				BulletRigidBody* BulletPhysicEngine::CreateBulletRigidBody( CollisionShapeData* infoC, RigidBodyData* infoR ) const
				{
					BulletRigidBody* rigidBody;
					btDefaultMotionState* motionState = new btDefaultMotionState();
					BulletCollisionShape* collisionShape = CreateBulletCollisionShape( infoC->m_shape );
					btRigidBody::btRigidBodyConstructionInfo infoRB( infoR->m_mass, motionState, collisionShape, TobtVector3(infoR->m_inertia) );

					infoRB.m_friction = infoR->m_friction;
					infoRB.m_restitution = infoR->m_restitution;

					rigidBody = new BulletRigidBody( infoRB );

					return rigidBody;
				}

				CollisionShape* BulletPhysicEngine::CoreCreateCollisionShape( CollisionShapeData* shapeInfo )
				{
					BulletAPICollisionShape* collisionShape = new BulletAPICollisionShape( shapeInfo );
					collisionShape->SetBulletCollisionShape( CreateBulletCollisionShape( shapeInfo->m_shape ) );
					return collisionShape;
				}

				void BulletPhysicEngine::CoreDeleteCollisionShape( CollisionShape* shape )
				{
					delete shape;
				}

				RigidBody* BulletPhysicEngine::CoreCreateRigidBody( RigidBodyData* rigidBodyInfo, CollisionShapeData* shapeInfo )
				{
					// Create rigid body
					BulletAPIRigidBody* rigidBody = new BulletAPIRigidBody( rigidBodyInfo, shapeInfo ); // Bullet CS not created
					rigidBody->SetBulletRigidBody( CreateBulletRigidBody( shapeInfo, rigidBodyInfo ) ); // Bullet RB has CS
					dynamic_cast<BulletAPICollisionShape*>(rigidBody->GetCollisionShape())->SetBulletCollisionShape( rigidBody->GetBulletRigidBody()->getCollisionShape() );

					rigidBody->IsKinematic( rigidBodyInfo->m_isKinematic );

					// Add to the world and push back
					m_dynamicWorld->addRigidBody( rigidBody->GetBulletRigidBody() );

					rigidBody->GetBulletRigidBody()->setUserPointer( dynamic_cast<RigidBody*>(rigidBody) );
					rigidBody->m_collisionData = new CollisionData;

					return rigidBody;
				}

				void BulletPhysicEngine::CoreDeleteRigidBody( RigidBody* rigidBody )
				{
					BulletAPIRigidBody* bulletRigidBody = dynamic_cast<BulletAPIRigidBody*>(rigidBody);
					m_dynamicWorld->removeRigidBody( bulletRigidBody->GetBulletRigidBody() );
				}

				void BulletPhysicEngine::CoreSetGravity( const Math::Vec3& gravity )
				{
					m_dynamicWorld->setGravity( TobtVector3(gravity) );
				}

				void BulletPhysicEngine::CoreDeactivateRigidBody( RigidBody* rigidBody )
				{
					BulletAPIRigidBody* bulletRigidBody = dynamic_cast<BulletAPIRigidBody*>(rigidBody);
					m_dynamicWorld->removeRigidBody( bulletRigidBody->GetBulletRigidBody() );
				}

				void BulletPhysicEngine::CoreActivateRigidBody( RigidBody* rigidBody )
				{
					BulletAPIRigidBody* bulletRigidBody = dynamic_cast<BulletAPIRigidBody*>(rigidBody);
					m_dynamicWorld->addRigidBody( bulletRigidBody->GetBulletRigidBody() );
				}
			}
		}
	}
}
