#include "TCMPhysicEngine/PhysicEngine/CollisionShape.h"

TCM::Engine::Math::Vec3 TCM::Engine::Physics::SCollisionShape::GetScale() const
{
	return m_collisionShapeInfo->m_scale;
}

void TCM::Engine::Physics::SCollisionShape::SetScale( Math::Vec3 scale )
{
	m_collisionShapeInfo->m_scaleChanged = true;
	m_collisionShapeInfo->m_scale = scale;
}

TCM::Engine::Math::Vec3 TCM::Engine::Physics::SCollisionShape::GetOffset() const
{
	return m_collisionShapeInfo->m_offset;
}

void TCM::Engine::Physics::SCollisionShape::SetOffset( Math::Vec3 offset )
{
	m_collisionShapeInfo->m_offset = offset;
}

TCM::Engine::Physics::CollisionShapesType TCM::Engine::Physics::SCollisionShape::GetShapeType() const
{
	return m_collisionShapeInfo->m_shape;
}

TCM::Engine::Physics::CollisionShapeData* TCM::Engine::Physics::SCollisionShape::GetCollisionShapeInfo()
{
	return m_collisionShapeInfo;
}

void TCM::Engine::Physics::SCollisionShape::SetCollisionShapeInfo( CollisionShapeData* bodyInfo )
{
	if ( m_collisionShapeInfo )
		delete m_collisionShapeInfo;
	m_collisionShapeInfo = bodyInfo;
}
