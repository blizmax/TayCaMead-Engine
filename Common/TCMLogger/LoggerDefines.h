#pragma once

#ifdef TCMLOGGER_EXPORTS
#define TCMLOGGER_DLL	__declspec(dllexport)

#ifdef _DEBUG
#include <vld.h>
#endif

#else
#define TCMLOGGER_DLL	__declspec(dllimport)
#endif
