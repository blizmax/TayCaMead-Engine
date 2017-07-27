#include "TCMComponent.h"
#include "TCMSceneObject.h"
#include "MathLib/Mat4.h"

namespace TCM
{
	namespace Engine
	{
		TCMComponent::TCMComponent()
			: m_sceneObject( nullptr )
		{
			SetName( "TCMComponent" );
		}

		TCMComponent::~TCMComponent()
		{
		}

		void TCMComponent::TCMPoolCreate()
		{
			m_bInit = false;
			m_bNeedDestroy = false;

			m_sceneObject = nullptr;
			SetEnabled( true, false );
		}

		void TCMComponent::TCMPoolRelease()
		{
			RemoveFromSceneObject();

			if ( !HasToBeDestroyed() )
				TCMDestroy();
		}

		Math::Mat4 TCMComponent::GetWorldMatrix() const
		{
			if ( m_sceneObject != nullptr )
				return m_sceneObject->GetWorldMatrix();
			return Math::Mat4::Identity();
		}

		void TCMComponent::SetSceneObject( TCMSceneObject* sceneObject )
		{
			if ( sceneObject == m_sceneObject )
				return;

			RemoveFromSceneObject();

			m_sceneObject = sceneObject;
			SetEnabled( m_sceneObject->IsEnabled() );
			//sceneObject->AddComponent( this );
		}

		void TCMComponent::RemoveFromSceneObject()
		{
			if ( m_sceneObject != nullptr )
			{
				TCMSceneObject* sceneObject = m_sceneObject;
				m_sceneObject = nullptr;
				sceneObject->RemoveComponent( this );
				SetEnabled( false );
			}
		}

		void TCMComponent::OnPositionChanged( const Math::Vec3& position )
		{
		}

		void TCMComponent::OnRotationChanged( const Math::Vec3& rotation )
		{
		}

		void TCMComponent::OnScaleChanged( const Math::Vec3& scale )
		{
		}

		void TCMComponent::OnWorldMatrixChanged( const Math::Mat4& mat )
		{
		}

		void TCMComponent::OnDestroy()
		{
			RemoveFromSceneObject();
		}

		void TCMComponent::RenderGizmos()
		{
			OnDrawGizmos();
			if ( m_sceneObject->m_isSelected )
				OnDrawGizmosSelected();
		}

		void TCMComponent::OnDrawGizmos()
		{
		}

		void TCMComponent::OnDrawGizmosSelected()
		{
		}

		void TCMComponent::OnCollisionEnter( Physics::RigidBody* body )
		{
		}

		void TCMComponent::OnCollisionExit()
		{
		}

		bool TCMComponent::SceneObjIsSelected() const
		{
			return m_sceneObject->m_isSelected;
		}
	}
}
