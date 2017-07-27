#include "EngineClasses\TCMBoxRigidbody.h"
#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMComponentMesh.h"
#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMBoxRigidbody )

		TCMBoxRigidbody::TCMBoxRigidbody()
		{
			m_collisionShapeInfo = new Engine::Physics::CollisionShapeData( Engine::Physics::BOX );
			InitRigidbody();
			UpdateMesh();
		}

		void TCMBoxRigidbody::UpdateMesh()
		{
			m_mesh = Resources::ResourceManager::GetMesh( "Box " +
				std::to_string( m_size.z ) + " " +
				std::to_string( m_size.x ) + " " +
				std::to_string( m_size.y ) );
		}

		Math::Vec3 TCMBoxRigidbody::GetSize() const
		{
			return m_size;
		}

		void TCMBoxRigidbody::SetSize( const Math::Vec3& size )
		{
			m_size = size;
			UpdateMesh();
		}

		void TCMBoxRigidbody::Init()
		{
			Super::Init();
			TCMComponentMesh* mesh = m_sceneObject->GetComponent<TCMComponentMesh>();
			if ( mesh != nullptr )
			{
				Math::Bounds::Box box = mesh->GetRenderData().GetMesh()->GetBox();
				Math::Vec3 size( box.width, box.height, box.length );
				m_size = size;
				size = size * Physics::PhysicEngine::GetUnitConvertor();
				m_rigidBody->GetCollisionShape()->SetScale( size );
				m_collisionShapeInfo->m_scale = size;

				UpdateMesh();
			}
		}
	}
}
