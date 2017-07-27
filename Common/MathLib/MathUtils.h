#pragma once
#include "MathLibDefine.h"
#include "Vec3.h"
#include "Quat.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			namespace Utils
			{
				// TODO: fix error
				/*MATHLIB_DLL_EXPORT*/
				const float PI = 3.141592653589793238462643f;
				const float epsilon = 0.00001f;

				MATHLIB_DLL_EXPORT float ToRadian( const float& angle );
				MATHLIB_DLL_EXPORT float ToDegree( const float& angle );

				MATHLIB_DLL_EXPORT Vec3 ToRadian( const Vec3& vec );
				MATHLIB_DLL_EXPORT Vec3 ToDegree( const Vec3& vec );
				MATHLIB_DLL_EXPORT Quat ToRadian( const Quat& angle );
				MATHLIB_DLL_EXPORT Quat ToDegree( const Quat& angle );
			}
		}
	}
}
