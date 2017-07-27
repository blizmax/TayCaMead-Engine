#pragma once

#ifdef TCMINPUTMANAGER_EXPORTS
#define TCMINPUTMANAGER_DLL	__declspec(dllexport)

#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "TCMNativeWindow/TCMNativeWindow.lib")
#pragma comment (lib, "TCMWindow/TCMWindow.lib")
#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define TCMINPUTMANAGER_DLL	__declspec(dllimport)
#endif
