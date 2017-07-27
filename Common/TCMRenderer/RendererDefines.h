#pragma once

#ifdef TCMRENDERER_EXPORTS
#define TCMRENDERER_DLL	__declspec(dllexport)

#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "TCMLogger/TCMLogger.lib")

#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define TCMRENDERER_DLL __declspec(dllimport)
#endif
