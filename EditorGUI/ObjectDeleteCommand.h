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
		class ObjectDeleteCommand :
				public EditorCommand
		{
		public:
			ObjectDeleteCommand( Engine::TCMSceneObject* object );
			~ObjectDeleteCommand();

			void Execute() override;
			void Undo() override;
		private:
			Engine::TCMSceneObject* m_objectDeleted;
			Engine::TCMSceneObject* m_oldParent;

			int m_position;
		};
	}
}
