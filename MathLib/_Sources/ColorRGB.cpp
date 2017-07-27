#include "MathLib/ColorRGB.h"
#include "MathLib/Vec3.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			ColorRGB::ColorRGB()
				: r( 0.0f )
				, g( 0.0f )
				, b( 0.0f )
			{
			}

			ColorRGB::ColorRGB( const ColorRGB& old )
				: r( old.r )
				, g( old.g )
				, b( old.b )
			{
			}

			ColorRGB::ColorRGB( const float& _r, const float& _g, const float& _b )
				: r( ClampFloat( _r ) )
				, g( ClampFloat( _g ) )
				, b( ClampFloat( _b ) )
			{
			}

			ColorRGB::ColorRGB( const Vec3& vec )
				: r( vec.x )
				, g( vec.y )
				, b( vec.z )
			{
			}

			ColorRGB ColorRGB::operator+( const ColorRGB& _a ) const
			{
				return
				{
					r + _a.r,
					g + _a.g,
					b + _a.b
				};
			}

			bool ColorRGB::operator==( const ColorRGB& _a ) const
			{
				return
				(
					r == _a.r &&
					g == _a.g &&
					b == _a.b
				);
			}

			const float& ColorRGB::operator[]( const int index ) const
			{
				switch ( index )
				{
					case 0: return r;
					case 1: return g;
					case 2: return b;
					default:
						throw std::runtime_error( "ColorRGB::operator[]() - index must be [0|1|2], not [" + std::to_string( index ) + "]" );
				}
			}

			float& ColorRGB::operator[]( const int index )
			{
				switch ( index )
				{
					case 0: return r;
					case 1: return g;
					case 2: return b;
					default:
						throw std::runtime_error( "ColorRGB::operator[]() - index must be [0|1|2], not [" + std::to_string( index ) + "]" );
				}
			}

			Vec3 ColorRGB::ToVec3() const
			{
				return Vec3( r, g, b );
			}

			bool ColorRGB::IsBlack() const
			{
				return r == 0.0f && g == 0.0f && b == 0.0f;
			}

			ColorRGB ColorRGB::FromHexRGB( const uint32_t hexValue )
			{
				ColorRGB rgbColor;

				rgbColor.r = ( ( hexValue >> 16 ) & 0xFF ) / 255.0f; // Extract the RR byte
				rgbColor.g = ( ( hexValue >> 8 ) & 0xFF ) / 255.0f; // Extract the GG byte
				rgbColor.b = ( ( hexValue ) & 0xFF ) / 255.0f; // Extract the BB byte

				return rgbColor;
			}

			ColorRGB ColorRGB::Black()
			{
				return { 0.0f, 0.0f, 0.0f };
			}

			ColorRGB ColorRGB::White()
			{
				return
						{ COLOR_MAX, COLOR_MAX, COLOR_MAX };
			}

			ColorRGB ColorRGB::Red()
			{
				return
						{ COLOR_MAX, 0.0f, 0.0f };
			}

			ColorRGB ColorRGB::Green()
			{
				return
						{ 0.0f, COLOR_MAX, 0.0f };
			}

			ColorRGB ColorRGB::Blue()
			{
				return
						{ 0.0f, 0.0f, COLOR_MAX };
			}

			ColorRGB ColorRGB::Orange()
			{
				return
						{ COLOR_MAX, 150.0f, 0.0f };
			}

			ColorRGB ColorRGB::Yellow()
			{
				return
						{ COLOR_MAX, COLOR_MAX, 0.0f };
			}

			ColorRGB ColorRGB::Cyan()
			{
				return
						{ 0.0f, COLOR_MAX, COLOR_MAX };
			}

			float ColorRGB::ClampFloat( const float& value )
			{
				if ( value < 0.0f )
					return 0.0f;
				if ( value > 1.0f )
					return 1.0f;
				return value;
			}
		}
	}
}
