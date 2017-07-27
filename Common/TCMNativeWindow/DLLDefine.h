#pragma once

#ifdef TCMNATIVEWINDOW_EXPORTS
#define TCMNATIVEWINDOW_DLL	__declspec(dllexport)

#ifdef _DEBUG
#include <vld.h>
#endif

#else
#define TCMNATIVEWINDOW_DLL	__declspec(dllimport)
#endif
