#pragma once

#ifdef OBJECTLOADER_EXPORTS

#define OBJECTLOADER_DLL_EXPORT	__declspec(dllexport)

#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "TCMRenderer/TCMRenderer.lib")

#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define OBJECTLOADER_DLL_EXPORT	__declspec(dllimport)
#endif
