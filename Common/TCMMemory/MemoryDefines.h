#pragma once

#define MEMORY_ALWAYS_EXPORT __declspec(dllexport)

#ifdef TCMMEMORY_EXPORTS
#define MEMORY_DLL __declspec(dllexport)

#ifdef _DEBUG
//#include <vld.h>
#endif
#else
	#define MEMORY_DLL __declspec(dllimport)
#endif
