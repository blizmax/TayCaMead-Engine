#include "TCMPhysicEngine/PhysicEngine/RigidBody.h"
#include "TCMPhysicEngine/PhysicEngine/CollisionShape.h"
#include "TCMPhysicEngine/PhysicEngine/AxisLiberties.h"
#include "TCMPhysicEngine/PhysicEngine/RigidBodyData.h"

TCM::Engine::Physics::RigidBody::~RigidBody()
{
	delete m_rigidBodyInfo;
}

TCM::Engine::Physics::CollisionShape* TCM::Engine::Physics::RigidBody::GetCollisionShape() const
{
	return m_collisionShape;
}

float TCM::Engine::Physics::RigidBody::GetMass() const
{
	return m_rigidBodyInfo->m_mass;
}

void TCM::Engine::Physics::RigidBody::SetMass( float mass )
{
	m_rigidBodyInfo->m_mass = mass;
	m_rigidBodyInfo->m_massChanged = true;
}

TCM::Engine::Math::Vec3 TCM::Engine::Physics::RigidBody::GetInertia() const
{
	return m_rigidBodyInfo->m_inertia;
}

void TCM::Engine::Physics::RigidBody::SetInertia( Math::Vec3 inertia )
{
	m_rigidBodyInfo->m_inertia = inertia;
	m_rigidBodyInfo->m_inertiaChanged = true;
}

bool TCM::Engine::Physics::RigidBody::IsUsingGravity() const
{
	return m_rigidBodyInfo->m_useGravity;
}

void TCM::Engine::Physics::RigidBody::UseGravity( bool val )
{
	m_rigidBodyInfo->m_useGravity = val;
}

bool TCM::Engine::Physics::RigidBody::IsKinematic() const
{
	return m_rigidBodyInfo->m_isKinematic;
}

void TCM::Engine::Physics::RigidBody::IsKinematic( bool val )
{
	m_rigidBodyInfo->m_isKinematic = val;
	m_rigidBodyInfo->m_isKinematicChanged = true;
}

TCM::Engine::Physics::AxisLiberties TCM::Engine::Physics::RigidBody::GetLiberties() const
{
	return m_rigidBodyInfo->m_frozenLiberties;
}

void TCM::Engine::Physics::RigidBody::SetLiberties( const AxisLiberties& liberties )
{
	m_rigidBodyInfo->m_frozenLibertiesChanged = true;
	m_rigidBodyInfo->m_frozenLiberties = liberties;
}

TCM::Engine::Math::Quat TCM::Engine::Physics::RigidBody::GetRotation() const
{
	return m_rigidBodyInfo->m_rotation;
}

void TCM::Engine::Physics::RigidBody::SetRotation( const Math::Quat& rotation )
{
	m_rigidBodyInfo->m_rotation = rotation;
}

TCM::Engine::Math::Vec3 TCM::Engine::Physics::RigidBody::GetEulerAngles() const
{
	return m_rigidBodyInfo->m_eulerAngles;
}

void TCM::Engine::Physics::RigidBody::SetEulerAngles( const Math::Vec3& angles )
{
	m_rigidBodyInfo->m_eulerAngles = angles;
}

float TCM::Engine::Physics::RigidBody::GetFriction() const
{
	return m_rigidBodyInfo->m_friction;
}

void TCM::Engine::Physics::RigidBody::SetFriction( const float& friction )
{
	m_rigidBodyInfo->m_friction = friction;
}

float TCM::Engine::Physics::RigidBody::GetRestitution() const
{
	return m_rigidBodyInfo->m_restitution;
}

void TCM::Engine::Physics::RigidBody::SetRestitution( const float& restitution )
{
	m_rigidBodyInfo->m_restitution = restitution;
}

TCM::Engine::Physics::RigidBodyData* TCM::Engine::Physics::RigidBody::GetRigidBodyInfo()
{
	return m_rigidBodyInfo;
}

void TCM::Engine::Physics::RigidBody::SetRigidBodyInfo( TCM::Engine::Physics::RigidBodyData* bodyInfo )
{
	if ( m_rigidBodyInfo )
		delete m_rigidBodyInfo;
	m_rigidBodyInfo = bodyInfo;
}
