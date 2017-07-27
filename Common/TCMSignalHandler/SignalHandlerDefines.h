#pragma once

#ifdef TCMSIGNALHANDLER_EXPORTS
#define TCMSIGNALHANDLER_DLL	__declspec(dllexport)
#pragma comment (lib, "TCMLogger/TCMLogger.lib")

#ifdef _DEBUG
#include <vld.h>
#endif
#else
	#define TCMSIGNALHANDLER_DLL	__declspec(dllimport)
#endif
