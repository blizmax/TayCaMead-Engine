#include "EditorLifecycle.h"
#include "TCMTime/Time.h"
#include "TCMInputManager/InputManager.h"
#include "TCMPhysicEngine/PhysicEngine/PhysicEngine.h"
#include "TCMWindow/AbstractWindow.h"
#include "TCMSaveManager/SaveData.h"
#include "TCMRenderer/Camera/CameraManager.h"

namespace TCM
{
	namespace Engine
	{
		EditorLifecycle::EditorLifecycle( TCMScene* scene )
			: Lifecycle( scene )
			, m_bPlaying( false )
		{
		}

		EditorLifecycle::~EditorLifecycle()
		{
		}

		void EditorLifecycle::GameLoop()
		{
			if ( !m_currentScene )
				return;

			Time::UpdateFrame();

			UserIO::InputManager::Update();

			if ( m_bPlaying )
			{
				m_lag += Time::GetUnscaledDeltaTime();

				while ( m_lag >= Time::GetUnscaledFixedDeltaTime() )
				{
					m_currentScene->TCMFixedUpdate( Time::GetFixedDeltaTime() );
					Physics::PhysicEngine::Run( Time::GetFixedDeltaTime() );
					m_lag -= Time::GetUnscaledFixedDeltaTime();
				}
				m_currentScene->TCMUpdate( Time::GetDeltaTime() );
				m_updateEvent.Invoke();
			}
			else
			{
				m_currentScene->TCMEditorUpdate( Time::GetUnscaledDeltaTime() );
			}

			Graphics::Camera::CameraManager::Render( m_currentScene );
			Graphics::AbstractWindow::RenderFinish();

			HandleState();
		}

		void EditorLifecycle::HandleState()
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
				case GameState::NEXT_FRAME:
					m_newState = GameState::PAUSE;
					break;
				case GameState::PLAY:
					OnPlay();
					break;
				case GameState::PAUSE:
					OnPause();
					break;
				case GameState::STOP:
					OnStop();
					break;
				case GameState::CONTINUE:
				default:
					break;
			}
			m_newState = GameState::CONTINUE;
		}

		void EditorLifecycle::OnPlay()
		{
			m_bPlaying = true;
		}

		void EditorLifecycle::OnPause()
		{
			if ( m_lag > 0 )
				m_bPlaying = !m_bPlaying;
		}

		void EditorLifecycle::OnStop()
		{
			m_bPlaying = false;

			m_lag = 0.0f;
		}
	}
}
