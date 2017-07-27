#pragma once

#ifdef TCMAUDIOENGINE_EXPORTS
#define TCMAUDIOENGINE_DLL	__declspec(dllexport)

#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "MathLib/MathLib.lib")

#include "TCMLogger/Logger.h"

#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define TCMAUDIOENGINE_DLL	__declspec(dllimport)
#endif
