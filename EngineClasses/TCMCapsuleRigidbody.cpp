#include "EngineClasses\TCMCapsuleRigidbody.h"
#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMComponentMesh.h"
#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMCapsuleRigidbody )

		TCMCapsuleRigidbody::TCMCapsuleRigidbody()
		{
			m_collisionShapeInfo = new Engine::Physics::CollisionShapeData( Engine::Physics::CAPSULE );
			InitRigidbody();
			UpdateMesh();
		}

		void TCMCapsuleRigidbody::UpdateMesh()
		{
			m_mesh = Resources::ResourceManager::GetMesh( "Capsule " + std::to_string( m_height ) + " " + std::to_string( m_radius ) + " 24" );
		}

		float TCMCapsuleRigidbody::GetRadius() const
		{
			return m_radius;
		}

		void TCMCapsuleRigidbody::SetRadius( const float radius )
		{
			m_radius = radius;
			UpdateMesh();
		}

		float TCMCapsuleRigidbody::GetHeight() const
		{
			return m_height;
		}

		void TCMCapsuleRigidbody::SetHeight( const float height )
		{
			m_height = height;
			UpdateMesh();
		}

		void TCMCapsuleRigidbody::Init()
		{
			Super::Init();
			TCMComponentMesh* mesh = m_sceneObject->GetComponent<TCMComponentMesh>();
			if ( mesh != nullptr )
			{
				float radius = mesh->GetRenderData().GetBoundingSphere().radius;
				float boundingSphereRadius = radius * Physics::PhysicEngine::GetUnitConvertor();
				m_rigidBody->GetCollisionShape()->SetScale( Math::Vec3( boundingSphereRadius, boundingSphereRadius, boundingSphereRadius ) );
				m_collisionShapeInfo->m_scale = Math::Vec3( boundingSphereRadius, boundingSphereRadius, boundingSphereRadius );

				m_height = radius;
				m_radius = radius * 0.5f;
				UpdateMesh();
			}
		}
	}
}
