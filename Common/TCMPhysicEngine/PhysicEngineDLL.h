#pragma once

#ifdef TCMPHYSICENGINE_EXPORTS
#define TCMPHYSICENGINE_DLL	__declspec(dllexport)

#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "TCMTime/TCMTime.lib")
#pragma comment (lib, "TCMRenderer/TCMRenderer.lib")
#pragma comment (lib, "TCMInputManager/TCMInputManager.lib")

#include "TCMLogger/Logger.h"

#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define TCMPHYSICENGINE_DLL	__declspec(dllimport)
#endif
