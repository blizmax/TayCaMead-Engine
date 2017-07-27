#include "TCMComponentRigidBody.h"
#include "TCMSceneObject.h"
#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "TCMComponentMesh.h"
#include "TCMRenderer/Gizmo/Gizmo.h"

#pragma comment (lib, "TCMPhysicEngine/TCMPhysicEngine.lib")
namespace TCM
{
	namespace Engine
	{
		TCMComponentRigidBody::TCMComponentRigidBody()
		{
			SetName( "TCMComponentRigidBody" );
			m_sceneObject = nullptr;

			m_rigidBodyInfo = new Engine::Physics::RigidBodyData( 1.f, Engine::Physics::PhysicEngine::GetDefaultInertie(),
			                                                      Engine::Physics::PhysicEngine::GetDefaultFriction(),
			                                                      Engine::Physics::PhysicEngine::GetDefaultRestitution() );
		}

		TCMComponentRigidBody::~TCMComponentRigidBody()
		{
			if ( m_rigidBody )
			{
				Physics::PhysicEngine::DeleteRigidBody( m_rigidBody );
				m_rigidBody = nullptr;
			}
			else if ( m_rigidBodyInfo )
			{
				delete m_rigidBodyInfo;
				m_rigidBodyInfo = nullptr;
			}

			if ( m_collisionShapeInfo )
			{
				delete m_collisionShapeInfo;
				m_collisionShapeInfo = nullptr;
			}
		}

		void TCMComponentRigidBody::Init()
		{
			m_rigidBody->SetPosition( m_sceneObject->GetWorldPosition() );

			m_rigidBody->SetRotation( m_rigidBodyInfo->m_rotation );
			m_rigidBody->SetEulerAngles( m_rigidBodyInfo->m_eulerAngles );
		}

		void TCMComponentRigidBody::TCMPoolRelease()
		{
			if ( m_rigidBody )
				Physics::PhysicEngine::DeactivateRigidBody( m_rigidBody );
			Super::TCMPoolRelease();
		}

		void TCMComponentRigidBody::TCMPoolCreate()
		{
			if ( m_rigidBody )
				Physics::PhysicEngine::ActivateRigidBody( m_rigidBody );
			Super::TCMPoolCreate();
		}

		void TCMComponentRigidBody::OnDrawGizmos()
		{
			if ( !m_mesh )
				return;
			Debug::Gizmo::Color( Math::ColorRGBA::White() );
			Debug::Gizmo::DrawWiredMesh( m_mesh,
			                             m_sceneObject->GetWorldPosition(),
			                             m_sceneObject->GetWorldRotation(),
			                             m_sceneObject->GetWorldScale() );
		}

		void TCMComponentRigidBody::FixedUpdate( const float fixedDeltaTime )
		{
			Super::FixedUpdate( fixedDeltaTime );

			if ( m_rigidBody->m_collisionData )
				if ( m_rigidBody->m_collisionData->m_enteredInCollision )
					m_sceneObject->OnCollisionEnter( m_rigidBody->m_collisionData->m_collisionedBody );

			m_rigidBody->Update();

			m_rigidBodyInfo->m_rotation = m_rigidBody->GetRotation();
			m_rigidBodyInfo->m_eulerAngles = m_rigidBody->GetEulerAngles();

			m_sceneObject->SetPosition( m_rigidBody->GetPosition() - m_rigidBody->GetCollisionShape()->GetOffset() );
			m_sceneObject->SetRotation( m_rigidBody->GetEulerAngles() );
		}

		void TCMComponentRigidBody::OnDestroy()
		{
			Super::OnDestroy();
		}

		void TCMComponentRigidBody::InitRigidbody()
		{
			if ( m_collisionShapeInfo && !m_rigidBody )
				m_rigidBody = Physics::PhysicEngine::CreateRigidBody( m_rigidBodyInfo, m_collisionShapeInfo );
		}

		void TCMComponentRigidBody::Extend( const Math::Vec3 extend ) const
		{
			if ( m_rigidBody )
				m_rigidBody->GetCollisionShape()->SetScale( extend );
			else
			{
				m_collisionShapeInfo->m_scale = extend;
				m_collisionShapeInfo->m_scaleChanged = true;
			}
		}

		void TCMComponentRigidBody::Offset( const Math::Vec3 offset ) const
		{
			if ( m_rigidBody )
				m_rigidBody->GetCollisionShape()->SetOffset( offset );
			else
				m_collisionShapeInfo->m_offset = offset;
		}

		Physics::CollisionShape* TCMComponentRigidBody::GetCollisionShape() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetCollisionShape();
			TCMWARNING("Rigid Body is not yet created, so its collision shape");
			return nullptr;
		}

		float TCMComponentRigidBody::GetMass() const
		{
			if ( m_rigidBody )
				return m_rigidBodyInfo->m_mass;
			return m_rigidBodyInfo->m_mass;
		}

		void TCMComponentRigidBody::SetMass( float mass ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetMass( mass );
			else
			{
				m_rigidBodyInfo->m_mass = mass;
				m_rigidBodyInfo->m_massChanged = true;
			}
		}

		Math::Vec3 TCMComponentRigidBody::GetInertia() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetInertia();
			return m_rigidBodyInfo->m_inertia;
		}

		void TCMComponentRigidBody::SetInertia( const Math::Vec3& inertia ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetInertia( inertia );
			else
			{
				m_rigidBodyInfo->m_inertia = inertia;
				m_rigidBodyInfo->m_inertiaChanged = true;
			}
		}

		bool TCMComponentRigidBody::IsUsingGravity() const
		{
			if ( m_rigidBody )
				return m_rigidBody->IsUsingGravity();
			return m_rigidBodyInfo->m_useGravity;
		}

		void TCMComponentRigidBody::SetUseGravity( bool val ) const
		{
			if ( m_rigidBody )
				m_rigidBody->UseGravity( val );
			else
				m_rigidBodyInfo->m_useGravity = val;
		}

		bool TCMComponentRigidBody::IsKinematic() const
		{
			if ( m_rigidBody )
				return m_rigidBody->IsKinematic();
			return m_rigidBodyInfo->m_isKinematic;
		}

		void TCMComponentRigidBody::IsKinematic( bool val ) const
		{
			if ( m_rigidBody )
				m_rigidBody->IsKinematic( val );
			else
			{
				m_rigidBodyInfo->m_isKinematic = val;
				m_rigidBodyInfo->m_isKinematicChanged = true;
			}
		}

		const Physics::AxisLiberties& TCMComponentRigidBody::GetLiberties() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetLiberties();
			return m_rigidBodyInfo->m_frozenLiberties;
		}

		void TCMComponentRigidBody::SetLiberties( const Physics::AxisLiberties& liberties ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetLiberties( liberties );
			else
			{
				m_rigidBodyInfo->m_frozenLiberties = liberties;
				m_rigidBodyInfo->m_frozenLibertiesChanged = true;
			}
		}

		Math::Vec3 TCMComponentRigidBody::GetPosition() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetPosition();

			return m_sceneObject->GetWorldPosition() + m_collisionShapeInfo->m_offset;
		}

		void TCMComponentRigidBody::SetPosition( const Math::Vec3& position ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetPosition( position );
		}

		const Math::Quat& TCMComponentRigidBody::GetRotation() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetRotation();
			return m_rigidBodyInfo->m_rotation;
		}

		void TCMComponentRigidBody::SetRotation( const Math::Quat& rotation ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetRotation( rotation );
			m_rigidBodyInfo->m_rotation = rotation;
		}

		const Math::Vec3& TCMComponentRigidBody::GetEulerAngles() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetEulerAngles();
			return m_rigidBodyInfo->m_eulerAngles;
		}

		void TCMComponentRigidBody::SetEulerAngles( const Math::Vec3& angles ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetEulerAngles( angles );
			m_rigidBodyInfo->m_eulerAngles = angles;
		}

		float TCMComponentRigidBody::GetFriction() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetFriction();
			return m_rigidBodyInfo->m_friction;
		}

		void TCMComponentRigidBody::SetFriction( const float& friction ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetFriction( friction );
			else
				m_rigidBodyInfo->m_friction = friction;
		}

		float TCMComponentRigidBody::GetRestitution() const
		{
			if ( m_rigidBody )
				return m_rigidBody->GetRestitution();
			return m_rigidBodyInfo->m_restitution;
		}

		void TCMComponentRigidBody::SetRestitution( const float& restitution ) const
		{
			if ( m_rigidBody )
				m_rigidBody->SetRestitution( restitution );
			else
				m_rigidBodyInfo->m_restitution = restitution;
		}

		void TCMComponentRigidBody::OnEnable()
		{
			if ( m_rigidBody )
				Physics::PhysicEngine::ActivateRigidBody( m_rigidBody );
		}

		void TCMComponentRigidBody::OnDisable()
		{
			if ( m_rigidBody )
				Physics::PhysicEngine::DeactivateRigidBody( m_rigidBody );
		}

		void TCMComponentRigidBody::OnCollisionEnter( Physics::RigidBody* body )
		{
			//TCMDEBUG("YEAY");
		}

		void TCMComponentRigidBody::OnCollisionExit()
		{
		}

		void TCMComponentRigidBody::OnScaleChanged( const Math::Vec3& scale )
		{
			TCMComponentMesh* mesh = m_sceneObject->GetComponent<TCMComponentMesh>();
			if ( mesh != nullptr && m_rigidBody )
			{
				float boundingSphereRadius = mesh->GetRenderData().GetBoundingSphere().radius * Physics::PhysicEngine::GetUnitConvertor();
				m_rigidBody->GetCollisionShape()->SetScale( Math::Vec3( boundingSphereRadius, boundingSphereRadius, boundingSphereRadius ) );
				m_collisionShapeInfo->m_scale = Math::Vec3( boundingSphereRadius, boundingSphereRadius, boundingSphereRadius );
			}
		}
	}
}
