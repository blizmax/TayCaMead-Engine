#pragma once

#include "MathLibDefine.h"
#include <string>

#define COLOR_MAX 1.0f

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec3;

			struct MATHLIB_DLL_EXPORT ColorRGB
			{
				ColorRGB();
				ColorRGB( const ColorRGB& old );
				// Assuming values are between 0.0f and 1.0f
				ColorRGB( const float& _r, const float& _g, const float& _b );
				// Assuming values are correct (no calcul to do)
				ColorRGB( const Vec3& vec );

				/**
				* \brief Red ratio
				*/
				float r;

				/**
				* \brief Green ratio
				*/
				float g;

				/**
				* \brief Blue ratio
				*/
				float b;

				ColorRGB operator+( const ColorRGB& _a ) const;
				bool operator==( const ColorRGB& _a ) const;
				const float& operator[]( const int index ) const;
				float& operator[]( const int index );

				/**
				* \brief Convert this \c ColorRGB to a Vec3(r, g, b)
				* \return A new \c Vec3 from this ColorRGB
				*/
				Vec3 ToVec3() const;

				/**
				 * \brief Check if color is black, all colors equals 0
				 * \return 
				 */
				bool IsBlack() const;

				/**
				* \brief Creates a new \c ColorRGB from a RGB hex value
				* \param hexValue _IN_ The RGB hex value
				* \return The new \c ColorRGB
				*/
				static ColorRGB FromHexRGB( const uint32_t hexValue );

				/**
				* \brief Creates a black \c ColorRGB
				* \return Black \c ColorRGB
				*/
				static ColorRGB Black();

				/**
				* \brief Creates a white \c ColorRGB
				* \return White \c ColorRGB
				*/
				static ColorRGB White();

				/**
				* \brief Creates a red \c ColorRGB
				* \return Red \c ColorRGB
				*/
				static ColorRGB Red();

				/**
				* \brief Creates a green \c ColorRGB
				* \return Green \c ColorRGB
				*/
				static ColorRGB Green();

				/**
				* \brief Creates a blue \c ColorRGB
				* \return Blue \c ColorRGB
				*/
				static ColorRGB Blue();

				/**
				* \brief Creates a orange \c ColorRGB
				* \return Orange \c ColorRGB
				*/
				static ColorRGB Orange();

				/**
				* \brief Creates a yellow \c ColorRGB
				* \return Yellow \c ColorRGB
				*/
				static ColorRGB Yellow();

				/**
				* \brief Creates a cyan \c ColorRGB
				* \return Cyan \c ColorRGB
				*/
				static ColorRGB Cyan();

			protected:
				/**
				* \brief Clamps a value between 0 and 1
				* \param value _IN_ The base value
				* \return The clamped value
				*/
				static float ClampFloat( const float& value );
			};
		}
	}
}
