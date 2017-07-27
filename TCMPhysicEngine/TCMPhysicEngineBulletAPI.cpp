#include "TCMPhysicEngine\TCMPhysicEngineBulletAPI\TCMPhysicEngineBulletAPI.h"
#include "TCMLogger\TCMLogger.h"
#include "TCMTime\TCMTime.h"
#include "TCMRenderer\TCMAbstractRenderer.h"
#include "TCMInputManager/TCMInputManager.h"
#include "MathLib\Vec2.h"

TCMPhysicEngineBulletAPI::~TCMPhysicEngineBulletAPI()
{
	BaseShutdown();
}

void TCMPhysicEngineBulletAPI::BaseInitialize( isart::Vec3 gravity )
{
	TCMDEBUG("TCMPhysicEngineBulletAPI::BaseInitialize() - Initializing Physics Engine...");
	pBroadphase broadphase = new btDbvtBroadphase();
	pCollisionConfig colliConfig = new CollisionConfig();
	m_collisionConfiguration = colliConfig;
	pDispatcher dispatcher = new Dispatcher( colliConfig );
	pSolver solver = new Solver();

	m_dynamicWorld = new DynamicWorld( dispatcher, broadphase, solver, colliConfig );
	m_dynamicWorld->setGravity( TobtVector3( gravity ) );

	m_pRigidBodyList = new std::vector<pRigidBody>;
}

void TCMPhysicEngineBulletAPI::BaseRun()
{
	//m_dynamicWorld->stepSimulation( TCMTime::GetDeltaTime(), static_cast<int>(TCMTime::GetDeltaTime() / m_maxFPSTimePerFrame), m_maxFPSTimePerFrame );
	for ( pRigidBody object : *m_pRigidBodyList )
		if ( object->isDirty )
		{
			m_dynamicWorld->updateSingleAabb( object->m_bulletRigidBody );
			object->isDirty = false;
		}
}

void TCMPhysicEngineBulletAPI::BaseShutdown()
{
	TCMDEBUG("TCMPhysicEngineBulletAPI::BaseShutdown() - Destroying Physics Engine with Bullet API...");

	for ( std::vector<pRigidBody>::iterator it = m_pRigidBodyList->begin(); it != m_pRigidBodyList->end(); ++it )
	{
		pRigidBody oneBody = *it;
		if ( oneBody )
		{
			m_dynamicWorld->removeRigidBody( oneBody->m_bulletRigidBody );
			delete oneBody->m_bulletRigidBody->getMotionState();
			delete oneBody;
		}
	}

	delete m_pRigidBodyList;
	delete m_collisionConfiguration;
	delete m_dynamicWorld->getDispatcher();
	delete m_dynamicWorld->getConstraintSolver();
	delete m_dynamicWorld->getBroadphase();
	delete m_dynamicWorld;
}

pBulletCollisionShape TCMPhysicEngineBulletAPI::CreateBulletCollisionShape( ECollisionShapesType shape )
{
	switch ( shape )
	{
		case SPHERE:
			return new btSphereShape( 1.f );
		case PLANE:
			return new btStaticPlaneShape( btVector3( 0.f, 1.f, 0.f ), 1.f );
		case TRIANGLE:
			return new btTriangleShapeEx( btVector3( 1.f, 0.f, 0.f ), btVector3( 0.f, 1.f, 0.f ), btVector3( -1.f, 0.f, 0.f ) );
		case CAPSULE:
			return new btCapsuleShape( 1.f, 1.f );
		case BOX:
			return new btBoxShape( btVector3( 1.f, 1.f, 1.f ) );
		case CYLINDER:
			return new btCylinderShape( btVector3( 0.5f, 1.f, 0.5f ) );
		default:
			return new btEmptyShape();
	}
}

pBulletRigidBody TCMPhysicEngineBulletAPI::CreateBulletRigidBody( const ECollisionShapesType shape, const float& mass, const isart::Vec3& inertia, const isart::Vec3& initalPosition )
{
	pBulletRigidBody rigidBody;
	btDefaultMotionState* motionState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), TobtVector3( initalPosition ) ) );
	pBulletCollisionShape collisionShape = CreateBulletCollisionShape( shape );
	btRigidBody::btRigidBodyConstructionInfo info( mass, motionState, collisionShape, TobtVector3( inertia ) );
	rigidBody = new BulletRigidBody( info );
	return rigidBody;
}

pSAbstractCollisionShape TCMPhysicEngineBulletAPI::BaseCreateCollisionShape( ECollisionShapesType shape )
{
	pCollisionShape collisionShape = new SCollisionShape;
	collisionShape->m_shapeType = shape;
	collisionShape->m_bulletCollisionShape = CreateBulletCollisionShape( shape );
	return collisionShape;
}

void TCMPhysicEngineBulletAPI::BaseDeleteCollisionShape( pSAbstractCollisionShape shape )
{
	delete shape;
}

pSAbstractRigidBody TCMPhysicEngineBulletAPI::BaseCreateRigidBody( const ECollisionShapesType shape, const float& mass, const isart::Vec3& inertia, const isart::Vec3& initalPosition )
{
	pRigidBody rigidBody = new SRigidBody;
	rigidBody->m_bulletRigidBody = CreateBulletRigidBody( shape, mass, inertia, initalPosition );
	rigidBody->m_bulletCollisionShape = rigidBody->m_bulletRigidBody->getCollisionShape();
	rigidBody->m_bulletCollisionShape->calculateLocalInertia( mass, TobtVector3( inertia ) );

	rigidBody->m_inertia = inertia;
	rigidBody->m_mass = mass;
	rigidBody->m_shapeType = shape;
	rigidBody->m_position = initalPosition;
	rigidBody->m_bulletRigidBody->translate( TobtVector3( initalPosition ) );

	m_dynamicWorld->addRigidBody( rigidBody->m_bulletRigidBody );
	m_pRigidBodyList->push_back( rigidBody );

	return rigidBody;
}

void TCMPhysicEngineBulletAPI::BaseDeleteRigidBody( pSAbstractRigidBody rigidBody )
{
	pRigidBody bulletRigidBody = dynamic_cast<pRigidBody>(rigidBody);
	m_dynamicWorld->removeRigidBody( bulletRigidBody->m_bulletRigidBody );

	std::vector<pRigidBody>::iterator it = m_pRigidBodyList->begin();
	for ( ; it != m_pRigidBodyList->end(); ++it )
	{
		if ( *it == bulletRigidBody )
		{
			m_pRigidBodyList->erase( it );
			break;
		}
	}

	delete bulletRigidBody->m_bulletRigidBody->getMotionState();
	delete bulletRigidBody->m_bulletCollisionShape;
	delete bulletRigidBody;
}

SAbstractCollisionShape TCMPhysicEngineBulletAPI::BaseFromScreenToWorldRayCast()
{
	ViewProjectionMatrix vprojMatrix = TCMAbstractRenderer::GetViewAndProjectionMatrices();

	isart::Vec2 mousePosition = TCMInputManager::GetMousePosition();
	CameraParams camPara = TCMAbstractRenderer::GetCameraParameters();

	isart::Vec3 normalisedDeviceCoor;
	normalisedDeviceCoor.x = 2.f * mousePosition.x / camPara.m_width - 1;
	normalisedDeviceCoor.y = 2.f * mousePosition.y / camPara.m_height - 1;
	normalisedDeviceCoor.z = 0.f;

	// Device coor -> View coor
	normalisedDeviceCoor = vprojMatrix.projectionMatrix.Inverted() * normalisedDeviceCoor;

	// View coor -> Model coor
	normalisedDeviceCoor = vprojMatrix.viewMatrix.Inverted() * normalisedDeviceCoor;

	TCMDEBUG("//////////////////////////////////////////////");

	isart::Vec3 cameraPosition = TCMAbstractRenderer::GetCameraParameters().m_position;

	TCMDEBUG("Cam: X:" + std::to_string(cameraPosition.x) + " - Y:" + std::to_string(cameraPosition.y) + " - Z:" + std::to_string(cameraPosition.z));

	isart::Vec3 ray = cameraPosition + ( normalisedDeviceCoor * 1000.f );

	isart::Vec3 from = cameraPosition;
	isart::Vec3 to = ray;

	btCollisionWorld::ClosestRayResultCallback callbackRay( TobtVector3( from ), TobtVector3( to ) );

	m_dynamicWorld->rayTest( TobtVector3( from ), TobtVector3( to ), callbackRay );

	TCMDEBUG("From: X:" + std::to_string(from.x) + " - Y:" + std::to_string(from.y) + " - Z:" + std::to_string(from.z));
	TCMDEBUG("To: X:" + std::to_string(to.x) + " - Y:" + std::to_string(to.y) + " - Z:" + std::to_string(to.z));

	if ( callbackRay.hasHit() )
	{
		TCMDEBUG("Was : ");
		TCMDEBUG(callbackRay.m_collisionObject->getCollisionShape()->getName());
		TCMDEBUG("At: ");
		TCMDEBUG( "X: " + std::to_string(callbackRay.m_hitPointWorld.x()) +
			"Y: " + std::to_string(callbackRay.m_hitPointWorld.y()) +
			"Z: " + std::to_string(callbackRay.m_hitPointWorld.z()));
	}

	TCMDEBUG("//////////////////////////////////////////////");
	return SAbstractCollisionShape();
}

void TCMPhysicEngineBulletAPI::BaseTestPhysic()
{
	ECollisionShapesType shape = ECollisionShapesType::BOX;
	float mass = 1.f;
	isart::Vec3 inertia = isart::Vec3( 0.f, 0.f, 0.f );

	pRigidBody sphereRigidBody = new SRigidBody( shape, mass, inertia );
	sphereRigidBody = dynamic_cast<pRigidBody>(BaseCreateRigidBody( shape, mass, inertia, isart::Vec3( 0.f, 50.f, 0.f ) ));

	/*btTransform sphereTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0.f, 50.f, 0.f));
	sphereRigidBody->m_bulletRigidBody->setWorldTransform(sphereTransform);*/

	shape = ECollisionShapesType::PLANE;
	mass = 0.f;

	pRigidBody planeRigidBody = new SRigidBody( shape, mass, inertia );
	planeRigidBody = dynamic_cast<pRigidBody>(BaseCreateRigidBody( shape, mass, inertia, isart::Vec3( 0.f, -1.f, 0.f ) ));


	for ( int i = 0; i < 300; ++i )
	{
		m_dynamicWorld->stepSimulation( 1 / 60.f, 10 );


		btTransform test;
		sphereRigidBody->m_bulletRigidBody->getMotionState()->getWorldTransform( test );

		TCMDEBUG("Y: " + std::to_string(test.getOrigin().getY()));
	}

	delete sphereRigidBody;
	delete planeRigidBody;
}

void TCMPhysicEngineBulletAPI::BaseSetGravity( isart::Vec3 gravity )
{
	m_gravity = gravity;
	m_dynamicWorld->setGravity( TobtVector3( gravity ) );
}
