#include "MathLib/Bounds.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			Bounds::Box Bounds::Sphere::ToBox() const
			{
				Box box;
				box.position = position;
				box.height = radius * 2.0f;
				box.width = box.height;
				box.length = box.width;

				return box;
			}

			bool Bounds::Sphere::IsInside( const Vec3& point ) const
			{
				return point.DistanceTo( position ) < radius;
			}

			bool Bounds::Sphere::IsColliding( const Sphere& s ) const
			{
				return s.position.DistanceTo( position ) < radius + s.radius;
			}

			Bounds::Sphere Bounds::Box::ToSphere() const
			{
				Sphere sphere;
				float radius = width;
				if ( radius < height )
					radius = height;
				if ( radius < length )
					radius = length;

				sphere.position = position;
				sphere.radius = radius / 2.0f;

				return sphere;
			}

			bool Bounds::Box::IsInside( const Vec3& point ) const
			{
				return abs( point.x - position.x ) < width
						&& abs( point.y - position.y ) < height
						&& abs( point.z - position.z ) < length;
			}

			Bounds::Box Bounds::Box::FromVec3( const Math::Vec3& size )
			{
				Box box;
				box.width = size.x;
				box.height = size.y;
				box.length = size.z;

				return box;
			}

			bool Bounds::IsInFrustum( const float frustum[24], const Sphere& sphere )
			{
				const float x = sphere.position.x;
				const float y = sphere.position.y;
				const float z = sphere.position.z;

				for ( int i = 0; i < 6; ++i )
				{
					float dist = frustum[i * 4] * x + frustum[i * 4 + 1] * y + frustum[i * 4 + 2] * z + frustum[i * 4 + 3];
					if ( dist <= -sphere.radius )
					{
						return false;
					}
				}
				return true;
			}

			bool Bounds::IsInFrustum( const float frustum[24], const Box& box )
			{
				Sphere sphere = box.ToSphere();

				const float x = sphere.position.x;
				const float y = sphere.position.y;
				const float z = sphere.position.z;

				for ( int i = 0; i < 6; ++i )
				{
					if ( frustum[i * 4] * x + frustum[i * 4 + 1] * y + frustum[i * 4 + 2] * z + frustum[i * 4 + 3] <= -sphere.radius )
						return false;
				}
				return true;
			}
		}
	}
}
