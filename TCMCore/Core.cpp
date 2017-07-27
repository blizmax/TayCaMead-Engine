#include "TCMCore/Core.h"

#include "TCMLogger/Logger.h"

#include "MathLib/Vec3.h"

#include "EngineClasses/Project.h"
#include "EngineClasses/TCMScene.h"
#include "EngineClasses/TCMSceneObject.h"

#include "EngineClasses/TCMDirectionnalLight.h"

#include "EngineClasses/TCMCameraComponent.h"
#include "EngineClasses/TCMComponentRigidBody.h"
#include "EngineClasses/TCMComponentMesh.h"

#include "EngineClasses/MusicComponent.h"
#include "EngineClasses/AudioListenerComponent.h"
#include "TCMAudioEngine/AudioEngine/AudioEngine.h"

#include "TCMInputManager/InputManager.h"
#include "TCMInputManager/BaseInputManagerKeys.h"

#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "TCMWindow//AbstractWindow.h"

#include "ObjectLoader/PrimitiveLoader.h"

#include "TCMSignalHandler/SignalHandler/SignalHandler.h"
#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TemporaryRotatingComponent.h"
#include "TCMNativeWindow/NativeWindow.h"
#include "EngineClasses/TCMSphereRigidbody.h"

#ifdef TCMEDITOR
#include "EditorLifecycle.h"
#else
#include "Lifecycle.h"
#endif

namespace TCM
{
	namespace Engine
	{
		Core& Core::GetInstance()
		{
			static std::unique_ptr<Core> m_instance;
			static std::once_flag m_onceFlag;

			std::call_once( m_onceFlag,
			                []()
			                {
				                m_instance.reset( new Core() );
			                }
			);
			return *( m_instance.get() );
		}

		Core::Core()
			: m_currentScene( nullptr )
			, m_currentProject( nullptr )
			, lifecycle( new CustomLifecycle( nullptr ) )
		{
			TCMINFO("Creating core...");
		}

		Core::~Core()
		{
			TCMINFO("Destroying core...");
			delete lifecycle;
		}

		void Core::Initialize()
		{
			GetInstance().CoreInitialize();
		}

		void Core::ResizeRenderer( const int width, const int height )
		{
			Graphics::AbstractWindow::ResizeCallback( width, height );
		}

		void Core::SetProject( Project* project )
		{
			GetInstance().CoreSetProject( project );
		}

		void Core::Run()
		{
			GetInstance().CoreRun();
		}

		void Core::RunExampleScene()
		{
			TCMScene* scene = GetExampleScene();
			ChangeScene( scene );
			Run();
		}

		void Core::RunOnce()
		{
			GetInstance().CoreRunOnce();
		}

		void Core::GenExampleScene()
		{
			GetInstance().CoreGenExampleScene();
		}

		void Core::ChangeScene( TCMScene* scene )
		{
			GetInstance().CoreChangeScene( scene );
		}

		void Core::ChangeScene( const int index )
		{
			GetInstance().CoreChangeScene( index );
		}

		void* Core::GetNativeWindow()
		{
			return Graphics::NativeWindow::GetWindow();
		}

		Event<>& Core::GetUpdateCallback()
		{
			return GetInstance().CoreGetUpdateCallback();
		}

		Event<>& Core::GetRenderCallback()
		{
			return Graphics::AbstractWindow::GetRenderEvent();
		}

		Event<>& Core::GetPlayEvent()
		{
			return GetInstance().m_OnPlay;
		}

		Event<>& Core::GetPauseEvent()
		{
			return GetInstance().m_OnPause;
		}

		Event<>& Core::GetStopEvent()
		{
			return GetInstance().m_OnStop;
		}

		Event<>& Core::GetNextFrameEvent()
		{
			return GetInstance().m_OnNextFrame;
		}

		TCMScene* Core::GetExampleScene()
		{
			return GetInstance().CoreGetExampleScene();
		}

		void Core::RefreshProjectSettings()
		{
			GetInstance().CoreRefreshProjectSettings();
		}

		void Core::Shutdown()
		{
			GetInstance().CoreShutdown();
		}

		void Core::EditorChangeScene( TCMScene* scene )
		{
			GetInstance().CoreEditorChangeScene( scene );
		}

		void Core::CoreInitialize()
		{
			TCMINFO("Initializing core...");

			Graphics::AbstractWindow::Init();
			Graphics::AbstractWindow::GetCloseEvent().Push( &Core::CoreShutdown, this );
			Graphics::NativeWindow::Init();

			auto signalHandler = []()
					{
						TCMFATALERROR("Terminating program due to raised signal.");
					};

			Signal::SignalHandler::RegisterSignalHandler( Signal::Signals::SEGMENTATION_FAULT, signalHandler );
			Signal::SignalHandler::RegisterSignalHandler( Signal::Signals::INTERRUPTION, signalHandler );
			Signal::SignalHandler::RegisterSignalHandler( Signal::Signals::ABORT, signalHandler );
			Signal::SignalHandler::RegisterSignalHandler( Signal::Signals::FLOATING_POINT_EXCEPTION, signalHandler );
			Signal::SignalHandler::RegisterSignalHandler( Signal::Signals::ILLEGAL_INSTRUCTION, signalHandler );
			Signal::SignalHandler::RegisterSignalHandler( Signal::Signals::TERMINATION, signalHandler );

			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_M );
#ifdef TCMEDITOR
			m_OnPlay.Push( &CustomLifecycle::Play, lifecycle );
			m_OnPause.Push( &CustomLifecycle::Pause, lifecycle );
			m_OnStop.Push( &CustomLifecycle::Stop, lifecycle );
			m_OnNextFrame.Push( &CustomLifecycle::NextFrame, lifecycle );
#endif
			UserIO::InputManager::GetKeyEvent( SSpecialKey::KEY_ESCAPE, SKey::NO_KEY_INPUT )
					.Push( &Core::Quit, this );

			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_Z )
					.Push( &Core::Play, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_X )
					.Push( &Core::Pause, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_C )
					.Push( &Core::Stop, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_V )
					.Push( &Core::NextFrame, this );
		}

		void Core::CoreSetProject( Project* project )
		{
			if ( project && project != m_currentProject )
			{
				if ( m_currentProject )
					delete m_currentProject;
				m_currentProject = project;
				RefreshProjectSettings();
			}
		}

		void Core::CoreGenExampleScene()
		{
			m_exampleScene = new TCMScene();

			Graphics::PostProcessEffect effects[] = { Graphics::PostProcessEffect::NONE, Graphics::PostProcessEffect::BLUR, Graphics::PostProcessEffect::EDGE };

			int count = 2;
			for ( int i = 0; i < count; i++ )
			{
				TCMSceneObject* obj = TCMSceneObject::Create();
				obj->SetPosition( Math::Vec3( 0.0f, 2.5f * ( i + 1 ), 10.0f * ( i + 1 ) ) );
				TCMCameraComponent* cameraComp = TCMComponent::Create<TCMCameraComponent>();
				cameraComp->GetCameraData()->SetDepth( i );
				cameraComp->GetCameraData()->SetRatio( Math::Vec2( 1.0f, 1.0f / count ) );
				cameraComp->GetCameraData()->m_position.y = ( 1.0f / count ) * i;
				cameraComp->GetCameraData()->AddPostProcessEffect( effects[i] );
				//if ( i == 0 )
				cameraComp->GetCameraData()->m_debugCamera = true;
				obj->AddComponent( cameraComp );

				AudioListenerComponent* audioComp = TCMComponent::Create<AudioListenerComponent>();
				obj->AddComponent( audioComp );

				m_exampleScene->AddSceneObject( obj );
			}

			Graphics::Camera::CameraManager::SetCameraCountOnScreen( count );

			//m_exampleScene->AddSceneObject( sphereObj );

			//first light
			for ( int i = 0; i < LIGHT_COUNT; ++i )
			{
				TCMSceneObject* lightObj = TCMSceneObject::Create();
				TCMDirectionnalLight* light = TCMComponent::Create<TCMDirectionnalLight>();
				lightObj->AddComponent( light );
				Math::Vec3 rotation;
				rotation.y += ( i + 1 ) / LIGHT_COUNT * 90;
				lightObj->SetRotation( rotation );
				m_exampleScene->AddSceneObject( lightObj );
			}

			// HACK: just for debug
			// Ratchet
			if ( RATCHET_OBJ )
			{
				int i = MAX_RATCHET;

				while ( i-- )
				{
					TCMSceneObject* tmp = Engine::TCMSceneObject::Create( "../Common/ObjectLoader/Ratchet/", "Ratchet.obj" );
					TCMComponent* rotatingComp = TCMComponent::Create<TemporaryRotatingComponent>();
					tmp->AddComponent( rotatingComp );
					tmp->SetPosition( Math::Vec3( 50.0f * i, 0.0f, 0.0f ) );
					m_exampleScene->AddSceneObject( tmp );
				}
			}

			// Jak
			if ( JAK_OBJ )
			{
				TCMSceneObject* tmp = Engine::TCMSceneObject::Create( "../Common/ObjectLoader/JIIJak/", "Jak.obj" );

				tmp->SetPosition( Math::Vec3( 40.0f, 0.0f, 0.0f ) );
				m_exampleScene->AddSceneObject( tmp );
			}

			if ( IRONMAN_FBX )
			{
				TCMSceneObject* tmp = Engine::TCMSceneObject::Create( "../Common/ObjectLoader/ironman/", "ironman.fbx" );

				tmp->SetPosition( Math::Vec3( 0.f, 200.f, -50.0f ) );
				tmp->SetScale( Math::Vec3( 20, 20, 20 ) );

				TCMSphereRigidbody* rigidBody = TCMComponent::Create<TCMSphereRigidbody>();
				rigidBody->IsKinematic( true );
				rigidBody->Extend( Math::Vec3( 5.f, 5.f, 5.f ) );
				//rigidBody->SetEulerAngles(TCM::Engine::Math::Vec3(0.f, 180.f, 180.f) );
				rigidBody->SetMass( 1 );
				rigidBody->SetFriction( 1.f );
				rigidBody->SetInertia( Math::Vec3( -1.f, -1.f, -1.f ) );
				rigidBody->IsKinematic( false );
				//rigidBody->Offset(TCM::Engine::Math::Vec3(0.f, 50.f, 0.f));
				Physics::AxisLiberties liberties;
				//liberties.m_posZ = false;
				rigidBody->SetLiberties( liberties );
				rigidBody->SetRestitution( 0.0f );
				//rigidBody->IsKinematic(false);
				tmp->AddComponent( rigidBody );

				tmp->SetPosition( Math::Vec3( 0.0f, 0.0f, 0.0f ) );
				tmp->SetScale( Math::Vec3( 5, 5, 5 ) );
				m_exampleScene->AddSceneObject( tmp );
			}

			if ( SPHERE_MESH )
			{
				for ( int i = 0; i < 2; ++i )
				{
					TCMSceneObject* sphere = TCMSceneObject::Create( "Sphere" );
					TCMComponentMesh* drawable = TCMComponent::Create<TCMComponentMesh>();
					drawable->SetMesh( Resources::TCMPrimitiveLoader::GenSphere( 20.f ) );
					sphere->SetPosition( Math::Vec3( 0.0f, 10 + i * 2.f, -i * 2.f ) );
					sphere->AddComponent( drawable );

					TCMSphereRigidbody* rigidBody = TCMComponent::Create<TCMSphereRigidbody>();
					rigidBody->SetMass( 0.1f );
					rigidBody->SetRestitution( i * 0.1f );
					sphere->AddComponent( rigidBody );

					MusicComponent* comp = TCMComponent::Create<MusicComponent>( Audio::pathToRessourcesInDebug + "music.ogg" );
					comp->SetPosition( Math::Vec3( 0.f, 0.f, 0.f ) );
					comp->SetAttenuation( 0.2f );
					comp->SetVolume( 80 );
					comp->SetLoop( true );
					comp->SetPlayOnInit( true );
					//comp->WarpSeekerAt(float (i*2));
					//comp->Play();
					sphere->AddComponent( comp );

					m_exampleScene->AddSceneObject( sphere );
				}
			}

			if ( CAPSULE_MESH )
			{
				TCMSceneObject* capsule = TCMSceneObject::Create( "Capsule" );
				TCMComponentMesh* drawable2 = TCMComponent::Create<TCMComponentMesh>( Resources::TCMPrimitiveLoader::GenCapsule( 10 ) );
				capsule->AddComponent( drawable2 );
				m_exampleScene->AddSceneObject( capsule );
			}

			if ( TORUS_MESH )
			{
				TCMSceneObject* capsule = TCMSceneObject::Create( "Torus" );
				TCMComponentMesh* drawable2 = TCMComponent::Create<TCMComponentMesh>( Resources::TCMPrimitiveLoader::GenTorus() );
				capsule->AddComponent( drawable2 );
				m_exampleScene->AddSceneObject( capsule );
			}

			if ( TUBE_MESH )
			{
				TCMSceneObject* capsule = TCMSceneObject::Create( "Tube" );
				TCMComponentMesh* drawable2 = TCMComponent::Create<TCMComponentMesh>( Resources::TCMPrimitiveLoader::GenTube() );
				capsule->AddComponent( drawable2 );
				m_exampleScene->AddSceneObject( capsule );
			}

			if ( CONE_MESH )
			{
				TCMSceneObject* capsule = TCMSceneObject::Create( "Cone" );
				TCMComponentMesh* drawable2 = TCMComponent::Create<TCMComponentMesh>( Resources::TCMPrimitiveLoader::GenCone() );
				capsule->AddComponent( drawable2 );
				m_exampleScene->AddSceneObject( capsule );
			}

			if ( CUBE_MESH )
			{
				TCMSceneObject* capsule = TCMSceneObject::Create( "Cube" );
				TCMComponentMesh* drawable2 = TCMComponent::Create<TCMComponentMesh>( Resources::TCMPrimitiveLoader::GenPlane() );
				capsule->AddComponent( drawable2 );
				m_exampleScene->AddSceneObject( capsule );
			}

			std::string skyfaces[6];

			skyfaces[0] = "../Common/TCMRenderer/skybox/right.jpg";
			skyfaces[1] = "../Common/TCMRenderer/skybox/left.jpg";
			skyfaces[2] = "../Common/TCMRenderer/skybox/top.jpg";
			skyfaces[3] = "../Common/TCMRenderer/skybox/bottom.jpg";
			skyfaces[4] = "../Common/TCMRenderer/skybox/back.jpg";
			skyfaces[5] = "../Common/TCMRenderer/skybox/front.jpg";

			m_exampleScene->SetSkyfaces( skyfaces );
		}

		void Core::CoreRun() const
		{
			TCMDEBUG( "Running core..." );

			while ( !m_bQuit )
			{
				lifecycle->GameLoop();
			}
		}

		void Core::CoreRunOnce() const
		{
			if ( !m_bQuit )
				lifecycle->GameLoop();
		}

		void Core::CoreShutdown()
		{
			TCMINFO("Shuting down core...");
			if ( m_currentScene )
			{
				m_currentScene->TCMDestroy();
				delete m_currentScene;
				m_currentScene = nullptr;
			}

			if ( m_currentProject )
			{
				delete m_currentProject;
				m_currentProject = nullptr;
			}

			if ( TCMBase::_mapClassMembers != nullptr )
			{
				delete TCMBase::_mapClassMembers;
				TCMBase::_mapClassMembers = nullptr;
			}
		}

		Event<>& Core::CoreGetUpdateCallback() const
		{
			return lifecycle->GetOnUpdateEvent();
		}

		void Core::CoreEditorChangeScene( TCMScene* scene )
		{
			if ( scene && scene != m_currentScene )
			{
				m_currentScene = scene;
				lifecycle->SetScene( m_currentScene, false );
			}
		}

		void Core::CoreChangeScene( TCMScene* scene )
		{
			if ( scene && scene != m_currentScene )
			{
				m_currentScene = scene;
				lifecycle->SetScene( m_currentScene );
			}
		}

		void Core::CoreChangeScene( const int index )
		{
			if ( m_currentProject )
				CoreChangeScene( m_currentProject->GetScenes()->at( index ) );
		}

		void Core::CoreRefreshProjectSettings() const
		{
			if ( m_currentProject )
			{
				Physics::PhysicEngine::SetGravity( m_currentProject->m_physicSettings.GetGravity() );
				Physics::PhysicEngine::SetDefaultFriction( m_currentProject->m_physicSettings.GetFriction() );
				Physics::PhysicEngine::SetDefaultRestitution( m_currentProject->m_physicSettings.GetRestitution() );
				Physics::PhysicEngine::SetDefaultInertie( m_currentProject->m_physicSettings.GetInertie() );

				Graphics::Camera::CameraManager::SetMSAASettings( m_currentProject->m_graphicSettings.GetMsaaEnabled(),
				                                                  m_currentProject->m_graphicSettings.GetMsaaSamples() );
				Graphics::RendererFactory::SetShadowParams( m_currentProject->m_graphicSettings.GetShadowMapSizeX(),
				                                            m_currentProject->m_graphicSettings.GetShadowMapSizeY(),
				                                            m_currentProject->m_graphicSettings.GetShadowNearPlane(),
				                                            m_currentProject->m_graphicSettings.GetShadowFarPlane() );
			}
		}

		TCMScene* Core::CoreGetExampleScene()
		{
			if ( !m_exampleScene )
				CoreGenExampleScene();
			return m_exampleScene;
		}

		void Core::Play()
		{
			m_OnPlay();
		}

		void Core::Pause()
		{
			m_OnPause();
		}

		void Core::Stop()
		{
			m_OnStop();
		}

		void Core::NextFrame()
		{
			m_OnNextFrame();
		}

		void Core::Quit()
		{
			m_bQuit = true;
			m_OnStop();
		}
	}
}
