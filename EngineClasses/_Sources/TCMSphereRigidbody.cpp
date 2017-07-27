#include "EngineClasses\TCMSphereRigidbody.h"
#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMComponentMesh.h"
#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMSphereRigidbody )

		TCMSphereRigidbody::TCMSphereRigidbody()
		{
			m_collisionShapeInfo = new Engine::Physics::CollisionShapeData( Engine::Physics::SPHERE );
			InitRigidbody();
			UpdateMesh();
		}

		float TCMSphereRigidbody::GetRadius() const
		{
			return m_radius;
		}

		void TCMSphereRigidbody::SetRadius( const float radius )
		{
			m_radius = radius;
			UpdateMesh();
		}

		void TCMSphereRigidbody::Init()
		{
			Super::Init();
			TCMComponentMesh* mesh = m_sceneObject->GetComponent<TCMComponentMesh>();
			if ( mesh != nullptr )
			{
				float radius = mesh->GetRenderData().GetBoundingSphere().radius;
				float boundingSphereRadius = radius * Physics::PhysicEngine::GetUnitConvertor();
				m_rigidBody->GetCollisionShape()->SetScale( Math::Vec3( boundingSphereRadius, boundingSphereRadius, boundingSphereRadius ) );
				m_collisionShapeInfo->m_scale = Math::Vec3( boundingSphereRadius, boundingSphereRadius, boundingSphereRadius );

				m_radius = radius;
				UpdateMesh();
			}
		}

		void TCMSphereRigidbody::UpdateMesh()
		{
			m_mesh = Resources::ResourceManager::GetMesh( "Sphere " + std::to_string( m_radius ) + " 24 16" );
		}
	}
}
