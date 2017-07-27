#pragma once

#include "ColorRGB.h"

#define COLOR_MAX 1.0f

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec3;
			struct Vec4;

			struct MATHLIB_DLL_EXPORT ColorRGBA : ColorRGB
			{
				ColorRGBA();
				ColorRGBA( const ColorRGBA& old );
				ColorRGBA( const ColorRGB& old );
				// Assuming values are between 0.0f and 1.0f
				ColorRGBA( const float& _r, const float& _g, const float& _b, const float& _a = 1.0f );
				// Assuming values are correct (no calcul to do)
				ColorRGBA( const Vec3& vec );
				// Assuming values are correct (no calcul to do)
				ColorRGBA( const Vec4& vec );

				/**
				 * \brief Alpha ratio
				 */
				float a;

				ColorRGBA operator+( const ColorRGBA& _a ) const;
				bool operator==( const ColorRGBA& _a ) const;
				const float& operator[]( const int index ) const;
				float& operator[]( const int index );

				/**
				 * \brief Convert this \c ColorRGBA to a Vec4(r, g, b, a)
				 * \return A new \c Vec4 from this ColorRGBA
				 */
				Vec4 ToVec4() const;

				/**
				 * \brief Creates a new \c ColorRGBA from a RGBA hex value
				 * \param hexValue _IN_ The RGBA hex value
				 * \return The new \c ColorRGBA
				 */
				static ColorRGBA FromHexRGBA( const uint32_t hexValue );

				/**
				 * \brief Creates a black \c ColorRGBA
				 * \return Black \c ColorRGBA
				 */
				static ColorRGBA Black();

				/**
				 * \brief Creates a white \c ColorRGBA
				 * \return White \c ColorRGBA
				 */
				static ColorRGBA White();

				/**
				 * \brief Creates a red \c ColorRGBA
				 * \return Red \c ColorRGBA
				 */
				static ColorRGBA Red();

				/**
				 * \brief Creates a green \c ColorRGBA
				 * \return Green \c ColorRGBA
				 */
				static ColorRGBA Green();

				/**
				 * \brief Creates a blue \c ColorRGBA
				 * \return Blue \c ColorRGBA
				 */
				static ColorRGBA Blue();

				/**
				 * \brief Creates a orange \c ColorRGBA
				 * \return Orange \c ColorRGBA
				 */
				static ColorRGBA Orange();

				/**
				 * \brief Creates a yellow \c ColorRGBA
				 * \return Yellow \c ColorRGBA
				 */
				static ColorRGBA Yellow();

				/**
				 * \brief Creates a cyan \c ColorRGBA
				 * \return Cyan \c ColorRGBA
				 */
				static ColorRGBA Cyan();

				/**
				 * \brief Creates a transparent \c ColorRGBA
				 * \return Transparent \c ColorRGBA
				 */
				static ColorRGBA Transparent();
			};
		}
	}
}
