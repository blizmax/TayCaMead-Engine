#include "ObjectInsertion.h"
#include "EngineClasses/TCMComponent.h"
#include "EngineClasses/TCMDirectionnalLight.h"
#include "EngineClasses/TCMSpotLight.h"
#include "EngineClasses/TCMPointLight.h"
#include "ObjectLoader/PrimitiveLoader.h"
#include "EngineClasses/SoundComponent.h"
#include "EngineClasses/MusicComponent.h"
#include "EngineClasses/TCMCameraComponent.h"
#include "EngineClasses/TCMComponentRigidBody.h"
#include "EngineClasses/TCMComponentMesh.h"
#include "MainWindow.h"
#include "EngineClasses/TCMScene.h"
#include "ObjectInsertCommand.h"
#include "EditorCommandManager.h"
#include "ObjectDeleteCommand.h"
#include "TCMSaveManager/BinarySerialization.hpp"
#include "TCMSaveManager/SceneObjectData.h"
#include "EngineClasses/TCMSphereRigidbody.h"
#include "EngineClasses/TCMBoxRigidbody.h"
#include "EngineClasses/TCMCapsuleRigidbody.h"
#include "../EngineClasses/TCMCylinderRigidbody.h"

namespace TCM
{
	namespace Editor
	{
		ObjectInsertion::ObjectInsertion( Engine::TCMSceneObject* parent )
			: m_parent( parent )
		{
			if ( !m_parent )
				m_parent = MainWindow::GetScene()->GetRootSceneObject();
		}

		void ObjectInsertion::Duplicate() const
		{
			if ( m_parent->GetParent() == nullptr )
				return;

			Data::SceneObjectData data( m_parent, nullptr );
			Engine::TCMSceneObject* object = data.Extract();

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent->GetParent(), object ) );
		}

		void ObjectInsertion::Paste( Data::SceneObjectData* data )
		{
			if ( data == nullptr )
				return;
			Engine::TCMSceneObject* object = data->Extract();
			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, object ) );
		}

		void ObjectInsertion::CreateEmptyObject() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateDirectionnalLight() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Directionnal Light" );
			Engine::TCMComponent* comp = Engine::TCMComponent::Create<Engine::TCMDirectionnalLight>();
			result->AddComponent( comp );
			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateSpotLight() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Spot Light" );
			Engine::TCMComponent* comp = Engine::TCMComponent::Create<Engine::TCMSpotLight>();
			result->AddComponent( comp );
			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreatePointLight() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			Engine::TCMComponent* comp = Engine::TCMComponent::Create<Engine::TCMPointLight>();
			result->AddComponent( comp );
			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateSphere() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Sphere" );
			Engine::TCMComponentMesh* comp = Engine::TCMComponent::Create<Engine::TCMComponentMesh>( Engine::Resources::TCMPrimitiveLoader::GenSphere() );
			result->AddComponent( comp );

			Engine::TCMSphereRigidbody* rigidBody = Engine::TCMComponent::Create<Engine::TCMSphereRigidbody>();
			result->AddComponent( rigidBody );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateCube() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Cube" );
			Engine::TCMComponentMesh* comp = Engine::TCMComponent::Create<Engine::TCMComponentMesh>( Engine::Resources::TCMPrimitiveLoader::GenPlane() );
			result->AddComponent( comp );

			Engine::TCMBoxRigidbody* rigidBody = Engine::TCMComponent::Create<Engine::TCMBoxRigidbody>();
			result->AddComponent( rigidBody );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreatePlane() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Plane" );
			Engine::TCMComponentMesh* comp = Engine::TCMComponent::Create<Engine::TCMComponentMesh>( Engine::Resources::TCMPrimitiveLoader::GenPlane() );
			result->AddComponent( comp );

			Engine::TCMBoxRigidbody* rigidBody = Engine::TCMComponent::Create<Engine::TCMBoxRigidbody>();
			result->AddComponent( rigidBody );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateCone() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Cone" );
			Engine::TCMComponentMesh* comp = Engine::TCMComponent::Create<Engine::TCMComponentMesh>( Engine::Resources::TCMPrimitiveLoader::GenCone() );
			result->AddComponent( comp );

			Engine::TCMBoxRigidbody* rigidBody = Engine::TCMComponent::Create<Engine::TCMBoxRigidbody>();
			result->AddComponent( rigidBody );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateTube() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Tube" );
			Engine::TCMComponentMesh* comp = Engine::TCMComponent::Create<Engine::TCMComponentMesh>( Engine::Resources::TCMPrimitiveLoader::GenTube() );
			result->AddComponent( comp );

			Engine::TCMCylinderRigidbody* rigidBody = Engine::TCMComponent::Create<Engine::TCMCylinderRigidbody>();
			result->AddComponent( rigidBody );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateCapsule() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Capsule" );
			Engine::TCMComponentMesh* comp = Engine::TCMComponent::Create<Engine::TCMComponentMesh>( Engine::Resources::TCMPrimitiveLoader::GenCapsule() );
			result->AddComponent( comp );

			Engine::TCMCapsuleRigidbody* rigidBody = Engine::TCMComponent::Create<Engine::TCMCapsuleRigidbody>();
			result->AddComponent( rigidBody );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateSoundSource() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Sound Source" );
			Engine::TCMComponent* comp = Engine::TCMComponent::Create<Engine::SoundComponent>();
			result->AddComponent( comp );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateMusicSource() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Music Source" );
			Engine::TCMComponent* comp = Engine::TCMComponent::Create<Engine::MusicComponent>();
			result->AddComponent( comp );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateMesh( const std::string& filepath ) const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create( filepath );
			result->SetName( "Mesh" );
			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreateCamera() const
		{
			Engine::TCMSceneObject* result = Engine::TCMSceneObject::Create();
			result->SetName( "Camera" );
			Engine::TCMComponent* comp = Engine::TCMComponent::Create<Engine::TCMCameraComponent>();
			result->AddComponent( comp );

			EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, result ) );
		}

		void ObjectInsertion::CreatePrefab( const std::string& filepath ) const
		{
			Data::SceneObjectData* objectData = BinarySerialization::Reader::LoadSceneObjectData( filepath );

			if ( objectData )
				EditorCommandManager::AddCommand( new ObjectInsertCommand( m_parent, objectData->Extract() ) );
		}

		void ObjectInsertion::SavePrefab( const std::string& filepath ) const
		{
			Data::SceneObjectData data( m_parent, nullptr );
			BinarySerialization::Writer::SaveSceneObjectData( &data, filepath );
		}

		void ObjectInsertion::DeleteObject() const
		{
			EditorCommandManager::AddCommand( new ObjectDeleteCommand( m_parent ) );
			MainWindow::SelectSceneObject( nullptr );
		}
	}
}
