#pragma once

#ifdef	TCMRESOURCEMANAGER_EXPORTS
#define TCMRESOURCEMANAGER_DLL	__declspec(dllexport)

#pragma comment (lib, "TCMLogger/TCMLogger.lib")
#pragma comment (lib, "TCMRenderer/TCMRenderer.lib")
#pragma comment (lib, "MathLib/MathLib.lib")
#pragma comment (lib, "ObjectLoader/ObjectLoader.lib")

#ifdef _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#else
	#define TCMRESOURCEMANAGER_DLL __declspec(dllimport)
#endif

// TODO: removed from here
class TCMRESOURCEMANAGER_DLL TCMAPoolObject
{
public:
	virtual ~TCMAPoolObject() = default;

	virtual void TCMPoolCreate() = 0;
	virtual void TCMPoolDelete() = 0;
};
