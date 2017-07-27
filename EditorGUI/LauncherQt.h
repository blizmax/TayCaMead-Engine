#pragma once
#include "Launcher.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class LauncherQt :
					public TCM::Editor::Launcher
			{
			public:
				int CoreLaunchEditor( int& argc, char** argv ) override;
			};
		}
	}
}
