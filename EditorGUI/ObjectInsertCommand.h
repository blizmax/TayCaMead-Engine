#pragma once
#include "EditorCommand.h"

namespace TCM
{
	namespace Engine
	{
		class TCMSceneObject;
	}
}

namespace TCM
{
	namespace Editor
	{
		class ObjectInsertCommand :
				public EditorCommand
		{
		public:
			ObjectInsertCommand( Engine::TCMSceneObject* parent, Engine::TCMSceneObject* child, int position = -1 );
			~ObjectInsertCommand();

			void Execute() override;
			void Undo() override;
		private:
			Engine::TCMSceneObject* m_parent;
			Engine::TCMSceneObject* m_child;

			int m_position;
		};
	}
}
