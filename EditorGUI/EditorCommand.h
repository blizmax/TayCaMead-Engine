#pragma once
namespace TCM
{
	namespace Editor
	{
		class EditorCommand abstract
		{
		public:
			virtual ~EditorCommand()
			{
			}

			virtual void Execute() = 0;
			virtual void Undo() = 0;
		};
	}
}
