#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/btBulletDynamicsCommon.h"
#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletRigidBody.h"
#include "TCMPhysicEngine/PhysicUtils.h"

TCM::Engine::Physics::Bullet::BulletAPIRigidBody::~BulletAPIRigidBody()
{
	delete m_collisionShape;
	delete m_bulletRigidBody->getMotionState();
	delete m_bulletRigidBody;
	delete m_collisionData;
}

TCM::Engine::Math::Vec3 TCM::Engine::Physics::Bullet::BulletAPIRigidBody::GetPosition() const
{
	btTransform transfrom = m_bulletRigidBody->getCenterOfMassTransform();
	Math::Vec3 origin = ToEngineVector3(transfrom.getOrigin());
	return origin;
}

void TCM::Engine::Physics::Bullet::BulletAPIRigidBody::SetPosition( Math::Vec3 position )
{
	if ( !IsKinematic() )
	{
		btTransform transfrom = m_bulletRigidBody->getCenterOfMassTransform();
		transfrom.setOrigin( TobtVector3(position) );
		m_bulletRigidBody->setCenterOfMassTransform( transfrom );
	}
	else
	{
		btTransform transfrom;
		m_bulletRigidBody->getMotionState()->getWorldTransform( transfrom );
		transfrom.setOrigin( TobtVector3(position) );
		m_bulletRigidBody->getMotionState()->setWorldTransform( transfrom );
	}
}

void TCM::Engine::Physics::Bullet::BulletAPIRigidBody::SetRotation( const Math::Quat& rotation )
{
	btTransform trans;
	trans = m_bulletRigidBody->getCenterOfMassTransform();
	trans.setRotation( TobtQuaternion(rotation) );
	m_bulletRigidBody->setCenterOfMassTransform( trans );

	RigidBody::SetRotation( rotation );
}

void TCM::Engine::Physics::Bullet::BulletAPIRigidBody::SetEulerAngles( const Math::Vec3& angles )
{
	btTransform trans;
	trans = m_bulletRigidBody->getCenterOfMassTransform();
	btQuaternion btQuat;
	Math::Quat isartQuat = Math::Quat::FromEulerAngle( angles );
	btQuat.setX( isartQuat.z );
	btQuat.setY( isartQuat.y );
	btQuat.setZ( isartQuat.x );
	btQuat.setW( isartQuat.w );
	trans.setRotation( btQuat );

	m_bulletRigidBody->setCenterOfMassTransform( trans );

	RigidBody::SetEulerAngles( angles );
}

float TCM::Engine::Physics::Bullet::BulletAPIRigidBody::GetFriction() const
{
	if ( m_bulletRigidBody )
		return m_bulletRigidBody->getFriction();
	return RigidBody::GetFriction();
}

void TCM::Engine::Physics::Bullet::BulletAPIRigidBody::SetFriction( const float& friction )
{
	m_bulletRigidBody->setFriction( friction );
	RigidBody::SetFriction( friction );
}

float TCM::Engine::Physics::Bullet::BulletAPIRigidBody::GetRestitution() const
{
	if ( m_bulletRigidBody )
		return m_bulletRigidBody->getRestitution();
	return RigidBody::GetRestitution();
}

void TCM::Engine::Physics::Bullet::BulletAPIRigidBody::SetRestitution( const float& restitution )
{
	m_bulletRigidBody->setRestitution( restitution );
	RigidBody::SetRestitution( restitution );
}

void TCM::Engine::Physics::Bullet::BulletAPIRigidBody::Update()
{
	m_rigidBodyInfo->m_eulerAngles += ToEngineVector3(m_bulletRigidBody->getAngularVelocity());
}

void TCM::Engine::Physics::Bullet::BulletAPIRigidBody::SetBulletRigidBody( BulletRigidBody* rigidBody )
{
	m_bulletRigidBody = rigidBody;
}

BulletRigidBody* TCM::Engine::Physics::Bullet::BulletAPIRigidBody::GetBulletRigidBody() const
{
	return m_bulletRigidBody;
}
