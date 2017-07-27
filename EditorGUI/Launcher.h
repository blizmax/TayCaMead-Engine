#pragma once
#include "EditorDefines.h"

namespace TCM
{
	namespace Editor
	{
		class Launcher abstract
		{
		public:
			Launcher() = default;
			virtual ~Launcher() = default;
			static int LaunchEditor( int& argc, char** argv );
			//static void OpenMainWindow();
			//static void OpenStartupWindow();
		protected:
			virtual int CoreLaunchEditor( int& argc, char** argv ) = 0;
		private:
			static Launcher& GetInstance();
		};
	}
}
