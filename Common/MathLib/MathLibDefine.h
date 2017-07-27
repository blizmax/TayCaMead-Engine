#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_DLL_EXPORT	__declspec(dllexport)

#include <cstdint>
#include <stdexcept>
#include <string>

#ifdef _DEBUG
#include <vld.h>
#endif

#else
	#define MATHLIB_DLL_EXPORT	__declspec(dllimport)
#endif
