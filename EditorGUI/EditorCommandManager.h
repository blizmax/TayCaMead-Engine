#pragma once
#include <vector>
#include "EditorComponent.h"
#include "TCMMemory/TCMEventManager.hpp"

namespace TCM
{
	namespace Editor
	{
		class EditorCommand;

		class EditorCommandManager
		{
		public:
			~EditorCommandManager() = default;

			static void SetIndex( const int index );
			static size_t Count();
			static void Clear( size_t start = 0 );
			static bool CanSave();
			static void Save();
			static bool CanUndo();
			static void Undo();
			static bool CanRedo();
			static void Redo();
			static void AddCommand( EditorCommand* command );

			static Engine::Event<>& GetOnAddCommandEvent();
			static Engine::Event<>& GetOnUndoRedoEvent();
			static void StaticRefresh( Engine::Project* project, Engine::TCMScene* scene );

		protected:
			static EditorCommandManager& GetInstance();

			EditorCommandManager() = default;

			void CoreSetIndex( const int index );
			void CoreClear( size_t start = 0 );
			void CoreUndo();
			void CoreRedo();
			void CoreAddCommand( EditorCommand* command );

			std::vector<EditorCommand *> m_commands;
			int m_index = -1;
			int m_lastSave = -1;

			Engine::Event<> m_OnAddCommand;
			Engine::Event<> m_OnUndoRedo;
		};
	}
}
