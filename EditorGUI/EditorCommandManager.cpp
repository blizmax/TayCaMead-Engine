#include "EditorCommandManager.h"
#include "EditorCommand.h"
#include <mutex>

namespace TCM
{
	namespace Editor
	{
		void EditorCommandManager::SetIndex( const int index )
		{
			GetInstance().CoreSetIndex( index );
		}

		size_t EditorCommandManager::Count()
		{
			return GetInstance().m_commands.size();
		}

		void EditorCommandManager::Clear( size_t start )
		{
			GetInstance().CoreClear( start );
		}

		bool EditorCommandManager::CanSave()
		{
			return GetInstance().m_lastSave != GetInstance().m_index;
		}

		void EditorCommandManager::Save()
		{
			GetInstance().m_lastSave = GetInstance().m_index;
		}

		bool EditorCommandManager::CanUndo()
		{
			return GetInstance().m_index >= 0;
		}

		void EditorCommandManager::Undo()
		{
			GetInstance().CoreUndo();
		}

		bool EditorCommandManager::CanRedo()
		{
			return GetInstance().m_index < static_cast<int>(Count()) - 1;
		}

		void EditorCommandManager::Redo()
		{
			GetInstance().CoreRedo();
		}

		void EditorCommandManager::AddCommand( EditorCommand* command )
		{
			GetInstance().CoreAddCommand( command );
		}

		Engine::Event<>& EditorCommandManager::GetOnAddCommandEvent()
		{
			return GetInstance().m_OnAddCommand;
		}

		Engine::Event<>& EditorCommandManager::GetOnUndoRedoEvent()
		{
			return GetInstance().m_OnUndoRedo;
		}

		void EditorCommandManager::StaticRefresh( Engine::Project* project, Engine::TCMScene* scene )
		{
			GetInstance().CoreClear();
		}

		EditorCommandManager& EditorCommandManager::GetInstance()
		{
			static std::unique_ptr<EditorCommandManager> m_instance;
			static std::once_flag m_onceFlag;

			std::call_once( m_onceFlag,
			                []()
			                {
				                m_instance.reset( new EditorCommandManager() );
			                }
			);

			return *m_instance.get();
		}

		void EditorCommandManager::CoreSetIndex( const int index )
		{
			if ( index < 0 || index >= m_commands.size() )
				return;
			if ( index > m_index )
				while ( index != m_index )
					CoreRedo();
			else if ( index < m_index )
				while ( index != m_index )
					CoreUndo();
		}

		void EditorCommandManager::CoreClear( size_t start )
		{
			if ( m_index >= start )
				m_index = start - 1;
			auto iter = m_commands.begin() + start;
			while ( iter != m_commands.end() )
			{
				delete *iter;
				iter = m_commands.erase( iter );
			}
		}

		void EditorCommandManager::CoreUndo()
		{
			m_commands[m_index--]->Undo();
			m_OnUndoRedo.Invoke();
		}

		void EditorCommandManager::CoreRedo()
		{
			m_commands[++m_index]->Execute();
			m_OnUndoRedo.Invoke();
		}

		void EditorCommandManager::CoreAddCommand( EditorCommand* command )
		{
			if ( !command )
				return;
			command->Execute();
			if ( m_index < m_commands.size() - 1 )
			{
				CoreClear( m_index + 1 );
			}
			m_commands.push_back( command );
			++m_index;
			m_OnAddCommand.Invoke();
		}
	}
}
