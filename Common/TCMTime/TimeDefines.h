#pragma once

#ifdef	TCMTIME_EXPORTS
#define TCMTIME_DLL	__declspec(dllexport)

#pragma comment (lib, "TCMLogger/TCMLogger.lib")

#ifdef _DEBUG
#include <vld.h>
#endif

#else
#define TCMTIME_DLL	__declspec(dllimport)
#endif
