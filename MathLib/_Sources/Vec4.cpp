#include "MathLib/Vec4.h"
#include "MathLib/Vec3.h"
#include "MathLib/MathUtils.h"
#include <cmath>

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			Vec4::Vec4() :
			             x( 0.0f )
			             , y( 0.0f )
			             , z( 0.0f )
			             , w( 0.0f )
			{
			}

			Vec4::Vec4( const Vec4& old ) :
			                              x( old.x )
			                              , y( old.y )
			                              , z( old.z )
			                              , w( old.w )
			{
			}

			Vec4::Vec4( const Vec3& old, const float& _w ) :
			                                               x( old.x )
			                                               , y( old.y )
			                                               , z( old.z )
			                                               , w( _w )
			{
			}

			Vec4::Vec4( const float& _x, const float& _y, const float& _z, const float& _w ) :
			                                                                                 x( _x )
			                                                                                 , y( _y )
			                                                                                 , z( _z )
			                                                                                 , w( _w )
			{
			}

			Vec4 Vec4::operator*( const float& value ) const
			{
				return { w * value,
					x * value,
					y * value,
					z * value };
			}

			Vec4 Vec4::operator+( const Vec4& a ) const
			{
				return { w + a.w,
					x + a.x,
					y + a.y,
					z + a.z };
			}

			Vec4 Vec4::operator-( const Vec4& a ) const
			{
				return { w - a.w,
					x - a.x,
					y - a.y,
					z - a.z };
			}

			bool Vec4::operator==( const Vec4& a ) const
			{
				return ( x > a.x - Utils::epsilon && x < a.x + Utils::epsilon &&
					y > a.y - Utils::epsilon && y < a.y + Utils::epsilon &&
					z > a.z - Utils::epsilon && z < a.z + Utils::epsilon &&
					w > a.w - Utils::epsilon && w < a.w + Utils::epsilon );
			}

			bool Vec4::operator!=( const Vec4& a ) const
			{
				return ( w != a.w ||
					x != a.x ||
					y != a.y ||
					z != a.z );
			}

			Vec4& Vec4::operator+=( const Vec4& a )
			{
				x += a.x;
				y += a.y;
				z += a.z;
				w += a.w;
				return *this;
			}

			Vec4& Vec4::operator-=( const Vec4& a )
			{
				x -= a.x;
				y -= a.y;
				z -= a.z;
				w -= a.w;
				return *this;
			}

			Vec4& Vec4::operator*=( const float& value )
			{
				x *= value;
				y *= value;
				z *= value;
				w *= value;
				return *this;
			}

			const float& Vec4::operator[]( const int index ) const
			{
				switch ( index )
				{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					case 3: return w;
					default:
						throw std::runtime_error( "Vec4::operator[]() - index must be [0|1|2|3], not [" + std::to_string( index ) + "]" );
				}
			}

			float& Vec4::operator[]( const int index )
			{
				switch ( index )
				{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					case 3: return w;
					default:
						throw std::runtime_error( "Vec4::operator[]() - index must be [0|1|2|3], not [" + std::to_string( index ) + "]" );
				}
			}

			float Vec4::Norm() const
			{
				return sqrt( NormSquared() );
			}

			float Vec4::NormSquared() const
			{
				return w * w + x * x + y * y + z * z;
			}

			void Vec4::Normalize()
			{
				const float norm = Norm();

				w = w / norm;
				x = x / norm;
				y = y / norm;
				z = z / norm;
			}

			Vec4 Vec4::Normalized() const
			{
				Vec4 result = Vec4( *this );
				result.Normalize();
				return result;
			}

			Vec4 Vec4::InnerProduct( const Vec4& a ) const
			{
				return { x * a.x, y * a.y, z * a.z, w * a.w };
			}

			Vec4 Vec4::Lerp( const Vec4& a, const Vec4& b, const float& t )
			{
				return a * ( 1.0f - t ) + b * t;
			}

			float Vec4::ScalarProduct( const Vec4& a, const Vec4& b )
			{
				return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
			}

			float Vec4::DistanceTo( const Vec4& to ) const
			{
				return ( Vec4( to.w - w, to.x - x, to.y - y, to.z - z ) ).Norm();
			}

			bool Vec4::IsZero() const
			{
				return ( w == 0.0f &&
					x == 0.0f &&
					y == 0.0f &&
					z == 0.0f );
			}

			Vec3 Vec4::ToVec3() const
			{
				return Vec3( x, y, z );
			}
		}
	}
}
