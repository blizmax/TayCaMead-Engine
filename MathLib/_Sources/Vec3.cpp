#include "MathLib/Vec3.h"
#include "MathLib/Vec4.h"
#include "MathLib/MathUtils.h"
#include <cmath>
#include <algorithm>

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			Vec3::Vec3() :
			             x( 0.0f )
			             , y( 0.0f )
			             , z( 0.0f )
			{
			}

			Vec3::Vec3( const Vec3& old ) :
			                              x( old.x )
			                              , y( old.y )
			                              , z( old.z )
			{
			}

			Vec3::Vec3( const Vec4& old ) :
			                              x( old.x )
			                              , y( old.y )
			                              , z( old.z )
			{
			}

			Vec3::Vec3( const float& _x, const float& _y, const float& _z ) :
			                                                                x( _x )
			                                                                , y( _y )
			                                                                , z( _z )
			{
			}

			Vec3 Vec3::operator*( const float& value ) const
			{
				return { x * value,
					y * value,
					z * value };
			}

			Vec3 Vec3::operator/( const float& value ) const
			{
				return { x / value,
					y / value,
					z / value };
			}

			Vec3 Vec3::operator+( const Vec3& a ) const
			{
				return { x + a.x,
					y + a.y,
					z + a.z };
			}

			Vec3 Vec3::operator-( const Vec3& a ) const
			{
				return { x - a.x,
					y - a.y,
					z - a.z };
			}

			bool Vec3::operator==( const Vec3& a ) const
			{
				return ( x > a.x - Utils::epsilon && x < a.x + Utils::epsilon &&
					y > a.y - Utils::epsilon && y < a.y + Utils::epsilon &&
					z > a.z - Utils::epsilon && z < a.z + Utils::epsilon );
			}

			bool Vec3::operator!=( const Vec3& a ) const
			{
				return ( x != a.x ||
					y != a.y ||
					z != a.z );
			}

			Vec3& Vec3::operator+=( const Vec3& a )
			{
				x += a.x;
				y += a.y;
				z += a.z;
				return *this;
			}

			Vec3& Vec3::operator-=( const Vec3& a )
			{
				x -= a.x;
				y -= a.y;
				z -= a.z;
				return *this;
			}

			Vec3& Vec3::operator*=( const float& value )
			{
				x *= value;
				y *= value;
				z *= value;
				return *this;
			}

			const float& Vec3::operator[]( const int index ) const
			{
				switch ( index )
				{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					default:
						throw std::runtime_error( "Vec3::operator[]() - index must be [0|1|2], not [" + std::to_string( index ) + "]" );
				}
			}

			float& Vec3::operator[]( const int index )
			{
				switch ( index )
				{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					default:
						throw std::runtime_error( "Vec3::operator[]() - index must be [0|1|2], not [" + std::to_string( index ) + "]" );
				}
			}

			Vec3 Vec3::Forward()
			{
				return { 0.0f, 0.0f, 1.0f };
			}

			Vec3 Vec3::Right()
			{
				return { 1.0f, 0.0f, 0.0f };
			}

			Vec3 Vec3::Up()
			{
				return { 0.0f, 1.0f, 0.0f };
			}

			float Vec3::Norm() const
			{
				return sqrt( NormSquared() );
			}

			float Vec3::NormSquared() const
			{
				return ( x * x + y * y + z * z );
			}

			void Vec3::Normalize()
			{
				const float norm = Norm();
				if ( norm == 0.0f )
					return;

				x = x / norm;
				y = y / norm;
				z = z / norm;
			}

			Vec3 Vec3::Normalized() const
			{
				Vec3 result = Vec3( *this );
				result.Normalize();
				return result;
			}

			Vec3 Vec3::InnerProduct( const Vec3& a ) const
			{
				return { x * a.x, y * a.y, z * a.z };
			}

			Vec3 Vec3::Lerp( const Vec3& a, const Vec3& b, const float& t )
			{
				return a * ( 1 - t ) + b * t;
			}

			float Vec3::ScalarProduct( const Vec3& a, const Vec3& b )
			{
				return a.x * b.x + a.y * b.y + a.z * b.z;
			}

			Vec3 Vec3::CrossProduct( const Vec3& a, const Vec3& b )
			{
				return { a.y * b.z - a.z * b.y,
					a.z * b.x - a.x * b.z,
					a.x * b.y - a.y * b.x };
			}

			float Vec3::DistanceTo( const Vec3& to ) const
			{
				return ( Vec3( to.x - x, to.y - y, to.z - z ) ).Norm();
			}

			float Vec3::DistanceSquaredTo( const Vec3& to ) const
			{
				return ( Vec3( to.x - x, to.y - y, to.z - z ) ).NormSquared();
			}

			float Vec3::GetBoxRadius() const
			{
				const float a = x * x + y * y;
				const float b = x * x + z * z;
				const float c = z * z + y * y;
				return sqrt( std::max( std::max( a, b ), c ) );
			}

			bool Vec3::IsZero() const
			{
				return ( x == 0.0f &&
					y == 0.0f &&
					z == 0.0f );
			}
		}
	}
}
