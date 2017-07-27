#pragma once

#ifdef TCMCORE_EXPORTS
#define TCMCORE_DLL	__declspec(dllexport)

#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "TCMTime/TCMTime.lib")
#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "TCMMemory/TCMMemory.lib")
#pragma comment (lib, "TCMWindow/TCMWindow.lib")
#pragma comment (lib, "TCMRenderer/TCMRenderer.lib")
#pragma comment (lib, "ObjectLoader/ObjectLoader.lib")
#pragma comment (lib, "EngineClasses/EngineClasses.lib")
#pragma comment (lib, "TCMSaveManager/TCMSaveManager.lib")
#pragma comment (lib, "TCMAudioEngine/TCMAudioEngine.lib")
#pragma comment (lib, "TCMInputManager/TCMInputManager.lib")
#pragma comment (lib, "TCMPhysicEngine/TCMPhysicEngine.lib")
#pragma comment (lib, "TCMSignalHandler/TCMSignalHandler.lib")
#pragma comment (lib, "TCMResourceManager/TCMResourceManager.lib")
#pragma comment (lib, "TCMNativeWindow/TCMNativeWindow.lib")

#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define TCMCORE_DLL	__declspec(dllimport)
#endif
