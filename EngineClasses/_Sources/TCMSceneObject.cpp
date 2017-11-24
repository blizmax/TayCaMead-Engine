#include "TCMSceneObject.h"

#include "TCMComponent.h"
#include "TCMComponentSubMesh.h"
#include "TCMMemory/TCMEventManager.hpp"
#include "TCMResourceManager/TCMResourceManager.h"
#include "TCMRenderer/Gizmo/Gizmo.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMSceneObject );

		Event<>& TCMSceneObject::GetOnRelationChange()
		{
			static Event<> event;
			return event;
		}

		TCMSceneObject::TCMSceneObject()
			: m_isSelected( false )
			, m_parent( nullptr )
			, m_vecChilds( nullptr )
			, m_vecComponent( nullptr )
		{
			// Set as a pointer and no reference to avoid warning and unexpected behaviours
			m_vecChilds = new std::vector<TCMSceneObject *>();
			m_vecComponent = new std::vector<TCMComponent *>();
		}

		TCMSceneObject::~TCMSceneObject()
		{
			for ( TCMSceneObject* child : *m_vecChilds )
			{
				if ( child != nullptr )
					Release( child );
			}
			delete m_vecChilds;

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				if ( oneComponent != nullptr )
					TCMComponent::Release( oneComponent );
			}
			delete m_vecComponent;
		}

		void TCMSceneObject::TCMPoolCreate()
		{
			m_bInit = false;
			m_bNeedDestroy = false;

			SetName( "TCMSceneObject" );
			m_vecChilds->clear();
			m_vecComponent->clear();

			m_transform.Reset();
			m_worldTransform.Reset();

			CalcWorldMatrix();
		}

		void TCMSceneObject::TCMPoolRelease()
		{
			if ( !HasToBeDestroyed() )
				TCMDestroy();

			for ( auto iter = m_vecChilds->begin(); iter != m_vecChilds->end(); ++iter )
			{
				Release( *iter );
			}
			m_vecChilds->clear();

			for ( auto iter = m_vecComponent->begin(); iter != m_vecComponent->end(); ++iter )
			{
				TCMComponent::Release( *iter );
			}
			m_vecComponent->clear();
		}

		void TCMSceneObject::Construct( const std::string& path )
		{
			Graphics::Skeleton* skeleton = Resources::ResourceManager::GetSkeleton( path );
			GenSkeleton( skeleton );
		}

		void TCMSceneObject::Construct( const std::string& path, const std::string& filename )
		{
			Graphics::Skeleton* skeleton = Resources::ResourceManager::GetSkeleton( path, filename );
			GenSkeleton( skeleton );
		}

		void TCMSceneObject::Construct( Graphics::Skeleton* skeleton )
		{
			GenSkeleton( skeleton );
		}

		void TCMSceneObject::GenSkeleton( Graphics::Skeleton* skeleton )
		{
			if ( skeleton == nullptr )
				return;
			for ( int i = 0; i < skeleton->GetSubMeshCount(); ++i )
			{
				TCMSceneObject* child = TCMSceneObject::Create();
				Graphics::SubMesh* subMesh = skeleton->GetSubMesh( i );

				TCMComponentSubMesh* comp = TCMComponent::Create<TCMComponentSubMesh>( subMesh );
				child->AddComponent( comp );
				child->SetPosition( subMesh->GetSphere().position );

				AddChild( child );
			}
		}

		void TCMSceneObject::Init()
		{
			Super::Init();

			CalcWorldMatrix();

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				if ( oneComponent->HasToBeInitialized() )
					oneComponent->Init();
			}

			for ( TCMSceneObject* child : *m_vecChilds )
			{
				if ( child->HasToBeInitialized() )
					child->Init();
			}
		}

		void TCMSceneObject::TCMEditorUpdate( const float deltaTime )
		{
			Super::TCMEditorUpdate( deltaTime );

			if ( m_transformCached != m_transform )
			{
				CalcWorldMatrix();
				m_transformCached = m_transform;
			}

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->TCMEditorUpdate( deltaTime );
			}

			for ( std::vector<TCMSceneObject *>::iterator iter = m_vecChilds->begin();
			      iter != m_vecChilds->end(); /* No auto incrementation */ )
			{
				TCMSceneObject* object = *iter;
				if ( object->HasToBeDestroyed() == true )
				{
					TCMSceneObject::Release( object );
					iter = m_vecChilds->erase( iter );
				}
				else if ( object->HasToBeInitialized() == true )
				{
					object->Init();
					++iter;
				}
				else
				{
					object->TCMEditorUpdate( deltaTime );
					++iter;
				}
			}
		}

		bool TCMSceneObject::TCMUpdate( const float deltaTime )
		{
			if ( !Super::TCMUpdate( deltaTime ) )
				return false;

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->TCMUpdate( deltaTime );
			}

			for ( std::vector<TCMSceneObject *>::iterator iter = m_vecChilds->begin();
			      iter != m_vecChilds->end(); /* No auto incrementation */ )
			{
				TCMSceneObject* object = *iter;
				if ( object->HasToBeDestroyed() == true )
				{
					TCMSceneObject::Release( object );
					iter = m_vecChilds->erase( iter );
				}
				else if ( object->HasToBeInitialized() == true )
				{
					object->Init();
					++iter;
				}
				else
				{
					object->TCMUpdate( deltaTime );
					++iter;
				}
			}

			return true;
		}

		bool TCMSceneObject::TCMFixedUpdate( const float fixedDeltaTime )
		{
			if ( !Super::TCMFixedUpdate( fixedDeltaTime ) )
				return false;

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->TCMFixedUpdate( fixedDeltaTime );
			}

			for ( TCMSceneObject* child : *m_vecChilds )
			{
				child->TCMFixedUpdate( fixedDeltaTime );
			}
			return true;
		}

		bool TCMSceneObject::TCMRender()
		{
			if ( !Super::TCMRender() )
				return false;

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->TCMRender();
			}

			for ( TCMSceneObject* child : *m_vecChilds )
			{
				child->TCMRender();
			}
			return true;
		}

		void TCMSceneObject::SetEnabled( const bool bEnabled, const bool callback )
		{
			Super::SetEnabled( bEnabled, callback );

			for ( auto iter = m_vecChilds->begin(); iter != m_vecChilds->end(); ++iter )
			{
				( *iter )->SetEnabled( bEnabled, callback );
			}

			for ( auto iter = m_vecComponent->begin(); iter != m_vecComponent->end(); ++iter )
			{
				( *iter )->SetEnabled( bEnabled, callback );
			}
		}

		void TCMSceneObject::RenderGizmos()
		{
			/*if ( !m_isSelected )
				return;*/

			Debug::Gizmo::Color( Math::ColorRGBA::Red() );
			Debug::Gizmo::DrawLine( m_worldTransform.position, m_worldTransform.position + ( Forward() * 3 ) );

			Debug::Gizmo::Color( Math::ColorRGBA::Blue() );
			Debug::Gizmo::DrawLine( m_worldTransform.position, m_worldTransform.position + ( Up() * 3 ) );

			Debug::Gizmo::Color( Math::ColorRGBA::Green() );
			Debug::Gizmo::DrawLine( m_worldTransform.position, m_worldTransform.position + ( Right() * 3 ) );
		}

		void TCMSceneObject::AddComponent( TCMComponent* component )
		{
			if ( component != nullptr )
			{
				component->SetSceneObject( this );
				m_vecComponent->push_back( component );
				component->OnPositionChanged( GetWorldPosition() );
				component->OnRotationChanged( GetWorldRotation() );
				component->OnScaleChanged( GetWorldScale() );
				component->OnWorldMatrixChanged( GetWorldMatrix() );
			}
			else
			TCMWARNING("Parameter component is null");
		}

		bool TCMSceneObject::RemoveComponent( TCMComponent* component ) const
		{
			if ( component != nullptr && !HasToBeDestroyed() )
			{
				for ( std::vector<TCMComponent *>::iterator it = m_vecComponent->begin(); it != m_vecComponent->end(); ++it )
				{
					if ( *it == component )
					{
						m_vecComponent->erase( it );
						component->RemoveFromSceneObject();
						return true;
					}
				}
			}
			else if ( component == nullptr )
			TCMWARNING("Parameter component is null");
			return false;
		}

		bool TCMSceneObject::RemoveComponent( const int index ) const
		{
			if ( index < 0 || index >= m_vecComponent->size() )
				return false;
			auto iter = m_vecComponent->begin() + index;
			TCMComponent* component = *iter;
			m_vecComponent->erase( iter );
			component->RemoveFromSceneObject();

			return true;
		}

		TCMComponent* TCMSceneObject::GetComponent( const int index ) const
		{
			if ( index < 0 || index >= m_vecComponent->size() )
				return nullptr;
			return ( *m_vecComponent )[index];
		}

		std::vector<TCMComponent*> TCMSceneObject::GetAllComponents() const
		{
			return *m_vecComponent;
		}

		size_t TCMSceneObject::GetComponentsCount() const
		{
			return m_vecComponent->size();
		}

		std::vector<TCMSceneObject*> TCMSceneObject::GetAllChildren() const
		{
			return *m_vecChilds;
		}

		int TCMSceneObject::GetChildIndex( TCMSceneObject* child ) const
		{
			std::vector<TCMSceneObject*> tcmSceneObjects = ( *m_vecChilds );
			for ( int i = 0; i < m_vecChilds->size(); ++i )
			{
				if ( tcmSceneObjects[i] == child )
					return i;
			}
			return -1;
		}

		std::vector<TCMSceneObject*> TCMSceneObject::GetChildren( const std::string& str ) const
		{
			std::vector<TCMSceneObject *> result;
			for ( std::vector<TCMSceneObject *>::iterator it = m_vecChilds->begin(); it != m_vecChilds->end(); ++it )
			{
				TCMSceneObject* object = *it;
				size_t position = object->GetName().find( str );

				if ( position != std::string::npos )
					result.push_back( object );
			}
			return result;
		}

		std::vector<TCMSceneObject*> TCMSceneObject::DeepGetChildren( const std::string& str ) const
		{
			std::vector<TCMSceneObject*> result;

			for ( std::vector<TCMSceneObject *>::iterator it = m_vecChilds->begin(); it != m_vecChilds->end(); ++it )
			{
				TCMSceneObject* object = *it;

				size_t position = object->GetName().find( str );
				if ( position != std::string::npos )
					result.push_back( object );

				std::vector<TCMSceneObject*> childList = object->DeepGetChildren( str );
				result.insert( result.end(), childList.begin(), childList.end() );
			}

			return result;
		}

		std::vector<TCMSceneObject*> TCMSceneObject::DeepGetAllChildren() const
		{
			std::vector<TCMSceneObject*> result;
			result.insert( result.end(), m_vecChilds->begin(), m_vecChilds->end() );

			for ( TCMSceneObject* object : *m_vecChilds )
			{
				std::vector<TCMSceneObject*> childList = object->DeepGetAllChildren();
				result.insert( result.end(), childList.begin(), childList.end() );
			}
			return result;
		}

		TCMSceneObject* TCMSceneObject::GetChild( const int index ) const
		{
			if ( index < 0 || index >= m_vecChilds->size() )
				return nullptr;
			return ( *m_vecChilds )[index];
		}

		size_t TCMSceneObject::GetChildCount() const
		{
			return m_vecChilds->size();
		}

		void TCMSceneObject::SetPosition( const Math::Vec3& position )
		{
			m_transform.position = position;
			CalcWorldMatrix();

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->OnPositionChanged( GetWorldPosition() );
			}
		}

		void TCMSceneObject::SetRotation( const Math::Vec3& rotation )
		{
			int rest = int( rotation.x ) % 360;
			m_transform.eulerAngles.x = rest + ( rotation.x - int( rotation.x ) );

			rest = int( rotation.y ) % 360;
			m_transform.eulerAngles.y = rest + ( rotation.y - int( rotation.y ) );

			rest = int( rotation.z ) % 360;
			m_transform.eulerAngles.z = rest + ( rotation.z - int( rotation.z ) );

			CalcWorldMatrix();

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				//TODO: Fix
				oneComponent->OnRotationChanged( GetWorldRotation() );
			}
		}

		void TCMSceneObject::SetScale( const Math::Vec3& scale )
		{
			m_transform.scale = scale;
			CalcWorldMatrix();

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->OnScaleChanged( GetWorldScale() );
			}
		}

		Math::Vec3 TCMSceneObject::GetLocalPosition() const
		{
			return m_transform.position;
		}

		Math::Vec3 TCMSceneObject::GetLocalRotation() const
		{
			return m_transform.eulerAngles;
		}

		Math::Vec3 TCMSceneObject::GetLocalScale() const
		{
			return m_transform.scale;
		}

		Math::Mat4 TCMSceneObject::GetWorldMatrix() const
		{
			return m_worldMatrix;
		}

		Math::Mat4 TCMSceneObject::GetLocaleMatrix() const
		{
			return Math::Mat4::CreateTransform( m_transform.position,
			                                    m_transform.eulerAngles,
			                                    m_transform.scale ).Transposed();
		}

		void TCMSceneObject::SetParent( TCMSceneObject* newParent )
		{
			if ( newParent == m_parent )
				return;

			RemoveParent();

			m_parent = newParent;

			//TODO: Remove that, it has to get his own enable
			SetEnabled( m_parent->IsEnabled() );

			GetOnRelationChange().Invoke();
			OnParentTransformChanged();
		}

		TCMSceneObject* TCMSceneObject::GetParent() const
		{
			return m_parent;
		}

		void TCMSceneObject::RemoveParent()
		{
			if ( m_parent != nullptr )
			{
				TCMSceneObject* oldParent = m_parent;
				m_parent = nullptr;
				oldParent->RemoveChild( this );
				SetEnabled( false );
				GetOnRelationChange().Invoke();
				OnParentTransformChanged();
			}
		}

		void TCMSceneObject::AddChild( TCMSceneObject* newChild, int position )
		{
			if ( newChild != nullptr )
			{
				newChild->SetParent( this );
				if ( position >= 0 && position < m_vecChilds->size() )
					m_vecChilds->insert( m_vecChilds->begin() + position, newChild );
				else
					m_vecChilds->push_back( newChild );
			}
			else
			TCMWARNING("Parameter sceneObject is null");
		}

		bool TCMSceneObject::RemoveChild( TCMSceneObject* oldChild ) const
		{
			if ( oldChild != nullptr && !HasToBeDestroyed() )
			{
				auto it = m_vecChilds->begin();
				for ( ; it != m_vecChilds->end(); ++it )
				{
					if ( *it == oldChild )
						break;
				}
				if ( it != m_vecChilds->end() )
				{
					TCMSceneObject* object = ( *it );
					m_vecChilds->erase( it );
					object->RemoveParent();
					return true;
				}
			}
			else if ( oldChild == nullptr )
			TCMWARNING("Parameter sceneObject is null");

			return false;
		}

		void TCMSceneObject::RemoveChild( const int index ) const
		{
			if ( index < 0 || index >= m_vecChilds->size() || HasToBeDestroyed() )
				return;

			auto iter = m_vecChilds->begin() + index;
			( *iter )->RemoveParent();
		}

		bool TCMSceneObject::RemoveChildren( const int start, const int count ) const
		{
			if ( start < 0 || start + count > m_vecChilds->size() || HasToBeDestroyed() )
				return false;
			auto iter = m_vecChilds->begin() + start;
			for ( int i = 0; i < count; ++i )
			{
				( *iter )->RemoveParent();
			}

			return true;
		}

		void TCMSceneObject::OnDestroy()
		{
			RemoveParent();

			for ( auto iter = m_vecChilds->begin(); iter != m_vecChilds->end(); ++iter )
			{
#ifdef _DEBUG
				if ( *iter )
#endif
					( *iter )->TCMDestroy();
#ifdef _DEBUG
				else
				TCMWARNING("An object in TCMSceneObject's updating children is nullptr !");
#endif
			}

			for ( auto iter = m_vecComponent->begin(); iter != m_vecComponent->end(); ++iter )
			{
#ifdef _DEBUG
				if ( *iter )
#endif
					( *iter )->TCMDestroy();
#ifdef _DEBUG
				else
				TCMWARNING("An object in TCMSceneObject's components is nullptr !");
#endif
			}
		}

		Math::Vec3 TCMSceneObject::GetWorldPosition() const
		{
			return m_worldTransform.position;
		}

		Math::Vec3 TCMSceneObject::GetWorldRotation() const
		{
			return m_worldTransform.eulerAngles;
		}

		Math::Vec3 TCMSceneObject::GetWorldScale() const
		{
			return m_worldTransform.scale;
		}

		void TCMSceneObject::OnParentTransformChanged()
		{
			CalcWorldMatrix();
			NotifyWorldMatChanged();
		}

		void TCMSceneObject::NotifyWorldMatChanged() const
		{
			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->OnPositionChanged( GetWorldPosition() );
				oneComponent->OnRotationChanged( GetWorldRotation() );
				oneComponent->OnScaleChanged( GetWorldScale() );
			}
		}

		void TCMSceneObject::OnCollisionEnter( Physics::RigidBody* body ) const
		{
			auto compIt = m_vecComponent->begin();
			auto compEnd = m_vecComponent->end();
			for ( ; compIt != compEnd; ++compIt )
				( *compIt )->OnCollisionEnter( body );

			auto childIt = m_vecChilds->begin();
			auto childEnd = m_vecChilds->end();
			for ( ; childIt != childEnd; ++childIt )
				( *childIt )->OnCollisionEnter( body );
		}

		void TCMSceneObject::OnCollisionExit() const
		{
			auto compIt = m_vecComponent->begin();
			auto compEnd = m_vecComponent->end();
			for ( ; compIt != compEnd; ++compIt )
				( *compIt )->OnCollisionExit();

			auto childIt = m_vecChilds->begin();
			auto childEnd = m_vecChilds->end();
			for ( ; childIt != childEnd; ++childIt )
				( *childIt )->OnCollisionExit();
		}

		bool TCMSceneObject::_RemoveChild( TCMSceneObject* oldChild ) const
		{
			auto it = m_vecChilds->begin();
			for ( ; it != m_vecChilds->end(); ++it )
			{
				if ( *it == oldChild )
					break;
			}
			if ( it != m_vecChilds->end() )
			{
				m_vecChilds->erase( it );
				return true;
			}

			return false;
		}

		void TCMSceneObject::CalcWorldMatrix()
		{
			Math::Mat4 worldMatrix = GetLocaleMatrix();

			if ( m_parent != nullptr )
				worldMatrix = m_parent->GetWorldMatrix() * worldMatrix;

			m_worldMatrix = worldMatrix;

			m_worldTransform.position = m_worldMatrix.GetTranslation( true );
			//TODO: Fix
			m_worldTransform.eulerAngles = m_worldMatrix.ExtractEulerAngles1( true );
			m_worldTransform.eulerAngles = GetLocalRotation();
			m_worldTransform.scale = m_worldMatrix.ExtractScale( true );

			for ( TCMSceneObject* child : *m_vecChilds )
			{
				child->OnParentTransformChanged();
			}

			for ( TCMComponent* oneComponent : *m_vecComponent )
			{
				oneComponent->OnWorldMatrixChanged( m_worldMatrix );
			}
		}
	}
}
