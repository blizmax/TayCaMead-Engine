#pragma once

#include "EngineClasses/TCMScene.h"
#include "TCMMemory/TCMEventManager.hpp"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief Class used for Lifecyle management
		 */
		class Lifecycle
		{
		protected:
			typedef int State;

			class EnumGameState
			{
			public:
				static const State NEED_EXIT = 0;
				static const State CONTINUE = 1;
				static const State CHANGE_SCENE = 2;
			};

		private:
			typedef EnumGameState GameState;

		public:
			explicit Lifecycle( TCMScene* scene );
			virtual ~Lifecycle();

			/**
			 * \brief Ask to change the scene for full cycle of life
			 * \param newScene _IN_ The scene to be used for future loops
			 * \param destruct _IN_OPT_ Destroy the old scene if necessary
			 */
			void SetScene( TCMScene* newScene, bool destruct = true );

			/**
			 * \brief Launches the game
			 */
			virtual void GameLoop();

			Event<>& GetOnUpdateEvent();

		protected:
			/**
			 * \brief Handle execution between to frames
			 */
			virtual void HandleState();

			/**
			 * \brief Change the currentScene
			 */
			virtual void OnChangeScene();

			/**
			 * \brief The current scene
			 */
			TCMScene* m_currentScene = nullptr;

			/**
			 * \brief The future scene when changing scenes
			 */
			TCMScene* m_futureScene = nullptr;

			/**
			 * \brief Last state the loop has gone by
			 */
			State m_lastState = GameState::CONTINUE;

			/**
			 * \brief New state the loop will handle
			 */
			State m_newState = GameState::CONTINUE;

			float m_lag = 0.0f;

			bool m_destruct = false;

			Event<> m_updateEvent;
		};
	}
}
