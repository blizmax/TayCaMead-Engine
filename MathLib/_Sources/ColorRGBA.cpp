#include "MathLib/ColorRGBA.h"
#include "MathLib/Vec4.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			ColorRGBA::ColorRGBA()
				: ColorRGB()
				, a( 1.0f )
			{
			}

			ColorRGBA::ColorRGBA( const ColorRGBA& old )
				: ColorRGB( old.r, old.g, old.b )
				, a( old.a )
			{
			}

			ColorRGBA::ColorRGBA( const ColorRGB& old )
				: ColorRGB( old.r, old.g, old.b )
				, a( 1.0f )
			{
			}

			ColorRGBA::ColorRGBA( const float& _r, const float& _g, const float& _b, const float& _a )
				: ColorRGB( ClampFloat( _r ), ClampFloat( _g ), ClampFloat( _b ) )
				, a( ClampFloat( _a ) )
			{
			}

			ColorRGBA::ColorRGBA( const Vec3& vec )
				: ColorRGB( vec )
				, a( 1.0f )
			{
			}

			ColorRGBA::ColorRGBA( const Vec4& vec )
				: ColorRGB( vec.x, vec.y, vec.z )
				, a( vec.w )
			{
			}

			ColorRGBA ColorRGBA::operator+( const ColorRGBA& _a ) const
			{
				return
				{
					r + _a.r,
					g + _a.g,
					b + _a.b,
					a + _a.a
				};
			}

			bool ColorRGBA::operator==( const ColorRGBA& _a ) const
			{
				return
				(
					r == _a.r &&
					g == _a.g &&
					b == _a.b &&
					a == _a.a
				);
			}

			const float& ColorRGBA::operator[]( const int index ) const
			{
				switch ( index )
				{
					case 0: return r;
					case 1: return g;
					case 2: return b;
					case 3: return a;
					default:
						throw std::runtime_error( "ColorRGBA::operator[]() - index must be [0|1|2|3], not [" + std::to_string( index ) + "]" );
				}
			}

			float& ColorRGBA::operator[]( const int index )
			{
				switch ( index )
				{
					case 0: return r;
					case 1: return g;
					case 2: return b;
					case 3: return a;
					default:
						throw std::runtime_error( "ColorRGBA::operator[]() - index must be [0|1|2|3], not [" + std::to_string( index ) + "]" );
				}
			}

			Vec4 ColorRGBA::ToVec4() const
			{
				return Vec4( r, g, b, a );
			}

			ColorRGBA ColorRGBA::FromHexRGBA( const uint32_t hexValue )
			{
				ColorRGBA rgbColor;

				rgbColor.r = ( ( hexValue >> 24 ) & 0xFF ) / 255.0f; // Extract the RR byte
				rgbColor.g = ( ( hexValue >> 16 ) & 0xFF ) / 255.0f; // Extract the GG byte
				rgbColor.b = ( ( hexValue >> 8 ) & 0xFF ) / 255.0f; // Extract the BB byte
				rgbColor.a = ( ( hexValue ) & 0xFF ) / 255.0f; // Extract the AA byte

				return rgbColor;
			}

			ColorRGBA ColorRGBA::Black()
			{
				return { 0.0f, 0.0f, 0.0f, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::White()
			{
				return
						{ COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::Red()
			{
				return
						{ COLOR_MAX, 0.0f, 0.0f, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::Green()
			{
				return
						{ 0.0f, COLOR_MAX, 0.0f, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::Blue()
			{
				return
						{ 0.0f, 0.0f, COLOR_MAX, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::Orange()
			{
				return
						{ COLOR_MAX, 150.0f, 0.0f, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::Yellow()
			{
				return
						{ COLOR_MAX, COLOR_MAX, 0.0f, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::Cyan()
			{
				return
						{ 0.0f, COLOR_MAX, COLOR_MAX, COLOR_MAX };
			}

			ColorRGBA ColorRGBA::Transparent()
			{
				return
						{ 0.0f, 0.0f, 0.0f, 0.0f };
			}
		}
	}
}
