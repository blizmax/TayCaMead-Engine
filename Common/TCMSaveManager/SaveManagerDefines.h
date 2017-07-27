#pragma once

#ifdef	TCMSAVEMANAGER_EXPORTS
#define TCMSAVEMANAGER_DLL	__declspec(dllexport)

#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "TCMMemory/TCMMemory.lib")
#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "EngineClasses/EngineClasses.lib")
#pragma comment (lib, "TCMPhysicEngine/TCMPhysicEngine.lib")

#ifdef _DEBUG
#include <vld.h>
#endif

#else
#define TCMSAVEMANAGER_DLL __declspec(dllimport)
#endif
