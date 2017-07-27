#pragma once

#include "Lifecycle.h"

namespace TCM
{
	namespace Engine
	{
		class EditorLifecycle : public Lifecycle
		{
		protected:
			class EnumEditorState : public EnumGameState
			{
			public:
				static const State PLAY = 3;
				static const State PAUSE = 4;
				static const State STOP = 5;
				static const State NEXT_FRAME = 6;
			};

		private:
			typedef EnumEditorState GameState;

		public:
			explicit EditorLifecycle( TCMScene* scene );
			virtual ~EditorLifecycle();

			virtual void GameLoop() override;

			void Play()
			{
				m_newState = GameState::PLAY;
			}

			void Pause()
			{
				m_newState = GameState::PAUSE;
			}

			void Stop()
			{
				m_newState = GameState::STOP;
			}

			void NextFrame()
			{
				m_newState = GameState::NEXT_FRAME;
			}

		protected:
			virtual void HandleState() override;

			void OnPlay();
			void OnPause();
			void OnStop();

			bool m_bPlaying = false;
		};
	}
}
