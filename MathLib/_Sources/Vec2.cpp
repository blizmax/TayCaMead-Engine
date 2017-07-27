#include "MathLib/Vec2.h"
#include "MathLib/MathUtils.h"
#include <cmath>

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			Vec2::Vec2() :
			             x( 0.0f )
			             , y( 0.0f )
			{
			}

			Vec2::Vec2( const Vec2& old ) :
			                              x( old.x )
			                              , y( old.y )
			{
			}

			Vec2::Vec2( const float& _x, const float& _y ) :
			                                               x( _x )
			                                               , y( _y )
			{
			}

			Vec2 Vec2::operator*( const float value ) const
			{
				return { x * value,
					y * value };
			}

			Vec2 Vec2::operator+( const Vec2& a ) const
			{
				return { x + a.x,
					y + a.y };
			}

			Vec2 Vec2::operator-( const Vec2& a ) const
			{
				return { x - a.x,
					y - a.y };
			}

			bool Vec2::operator==( const Vec2& a ) const
			{
				return ( x > a.x - Utils::epsilon && x < a.x + Utils::epsilon &&
					y > a.y - Utils::epsilon && y < a.y + Utils::epsilon );
			}

			bool Vec2::operator!=( const Vec2& a ) const
			{
				return ( x != a.x ||
					y != a.y );
			}

			Vec2& Vec2::operator+=( const Vec2& a )
			{
				x += a.x;
				y += a.y;
				return *this;
			}

			Vec2& Vec2::operator-=( const Vec2& a )
			{
				x -= a.x;
				y -= a.y;
				return *this;
			}

			Vec2& Vec2::operator*=( const float value )
			{
				x *= value;
				y *= value;
				return *this;
			}

			const float& Vec2::operator[]( const int index ) const
			{
				switch ( index )
				{
					case 0: return x;
					case 1: return y;
					default:
						throw std::runtime_error( "Vec2::operator[]() - index must be [0|1], not [" + std::to_string( index ) + "]" );
				}
			}

			float& Vec2::operator[]( const int index )
			{
				switch ( index )
				{
					case 0: return x;
					case 1: return y;
					default:
						throw std::runtime_error( "Vec2::operator[]() - index must be [0|1], not [" + std::to_string( index ) + "]" );
				}
			}

			Vec2 Vec2::Right()
			{
				return { 1.0f, 0.0f };
			}

			Vec2 Vec2::Up()
			{
				return { 0.0f, 1.0f };
			}

			float Vec2::Norm() const
			{
				return sqrt( NormSquared() );
			}

			float Vec2::NormSquared() const
			{
				return x * x + y * y;
			}

			void Vec2::Normalize()
			{
				const float norm = Norm();
				x = x / norm;
				y = y / norm;
			}

			Vec2 Vec2::Normalized() const
			{
				Vec2 result = Vec2( *this );
				result.Normalize();
				return result;
			}

			Vec2 Vec2::InnerProduct( const Vec2& a ) const
			{
				return { x * a.x, y * a.y };
			}

			Vec2 Vec2::Lerp( const Vec2& a, const Vec2& b, float t )
			{
				return a * ( 1 - t ) + b * t;
			}

			float Vec2::ScalarProduct( const Vec2& a, const Vec2& b )
			{
				return a.x * b.x + a.y * b.y;
			}

			float Vec2::DistanceTo( const Vec2& to ) const
			{
				return Vec2( to.x - x, to.y - y ).Norm();
			}

			bool Vec2::IsZero() const
			{
				return ( x == 0 &&
					y == 0 );
			}
		}
	}
}
