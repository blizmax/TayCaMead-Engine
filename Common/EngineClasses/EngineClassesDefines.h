#pragma once

#ifdef ENGINECLASSES_EXPORTS
#define ENGINECLASSES_DLL_EXPORT	__declspec(dllexport)

#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "TCMTime/TCMTime.lib")
#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "TCMMemory/TCMMemory.lib")
#pragma comment (lib, "TCMRenderer/TCMRenderer.lib")
#pragma comment (lib, "TCMInputManager/TCMInputManager.lib")
#pragma comment (lib, "TCMResourceManager/TCMResourceManager.lib")

#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define ENGINECLASSES_DLL_EXPORT	__declspec(dllimport)
#endif

#include "TCMLogger/Logger.h"
