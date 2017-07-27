#pragma once
#include <vector>
#include "MathLibDefine.h"
#include "MathLib/Vec3.h"


namespace isart
{
	class MATHLIB_DLL_EXPORT BoundingObjects
	{
	public:

		struct MATHLIB_DLL_EXPORT Sphere
		{
			isart::Vec3 position = { 0, 0, 0 };
			float radius = 0;
		};

		static Sphere GetBoundingSphere( const std::vector<isart::Vec3>& pointSet, const int maxId );
		static bool IsSphereInFrustum( const float frustum[24], const Sphere& sphere );

	private:
		static Sphere GetMiniBoundingSphere( const std::vector<isart::Vec3>& pointSet, const int maxId, std::vector<isart::Vec3>& boundarySet );
		static Sphere GetSphere( const isart::Vec3 p[3] );
	};
}
