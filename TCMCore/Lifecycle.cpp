#include "Lifecycle.h"
#include "TCMRenderer/Camera/CameraManager.h"
#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "TCMInputManager/InputManager.h"
#include "TCMTime/Time.h"
#include "EngineClasses/TCMScene.h"
#include "TCMWindow/AbstractWindow.h"

namespace TCM
{
	namespace Engine
	{
		Lifecycle::Lifecycle( TCMScene* scene )
			: m_currentScene( scene )
			, m_futureScene( nullptr )
			, m_lastState( GameState::CONTINUE )
			, m_newState( GameState::CONTINUE )
		{
			Physics::PhysicEngine::Initialize( Math::Vec3( 0.0f, -9.8f, 0.0f ) );
		}

		Lifecycle::~Lifecycle()
		{
			Physics::PhysicEngine::Shutdown();
		}

		void Lifecycle::SetScene( TCMScene* scene, bool destruct )
		{
			if ( !m_currentScene )
			{
				m_currentScene = scene;
				m_currentScene->Init();
				m_currentScene->SetEnabled( true );
			}
			else
			{
				m_futureScene = scene;
				m_destruct = destruct;
				// Do not Init(), will be done in ChangeScene()
				m_newState = GameState::CHANGE_SCENE;
			}
		}

		void Lifecycle::GameLoop()
		{
			if ( !m_currentScene )
				return;

			Time::UpdateFrame();

			m_lag += Time::GetUnscaledDeltaTime();

			while ( m_lag >= Time::GetUnscaledFixedDeltaTime() )
			{
				m_currentScene->TCMFixedUpdate( Time::GetFixedDeltaTime() );
				Physics::PhysicEngine::Run( Time::GetFixedDeltaTime() );
				m_lag -= Time::GetUnscaledFixedDeltaTime();
			}

			UserIO::InputManager::Update();
			m_currentScene->TCMUpdate( Time::GetDeltaTime() );
			m_updateEvent.Invoke();

			Graphics::Camera::CameraManager::Render( m_currentScene );
			Graphics::AbstractWindow::RenderFinish();

			HandleState();
		}

		Event<>& Lifecycle::GetOnUpdateEvent()
		{
			return m_updateEvent;
		}

		void Lifecycle::HandleState()
		{
			m_lastState = m_newState;

			switch ( m_newState )
			{
				case GameState::CHANGE_SCENE:
					OnChangeScene();
					break;
				case GameState::NEED_EXIT:
					// ExitLoop();
					break;
				case GameState::CONTINUE:
				default:
					break;
			}

			m_newState = GameState::CONTINUE;
		}

		void Lifecycle::OnChangeScene()
		{
			if ( m_currentScene )
			{
				m_currentScene->SetEnabled( false );
				if ( m_destruct )
				{
					m_currentScene->TCMDestroy();
					delete m_currentScene;
				}
			}

			if ( m_futureScene )
			{
				m_currentScene = m_futureScene;
				m_futureScene = nullptr;
				m_currentScene->Init();
				m_currentScene->SetEnabled( true );
			}
		}
	}
}
