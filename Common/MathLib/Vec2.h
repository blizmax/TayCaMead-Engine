#pragma once
#include "MathLibDefine.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct MATHLIB_DLL_EXPORT Vec2
			{
				Vec2();
				Vec2( const Vec2& old );
				Vec2( const float& _x, const float& _y );

				float x;
				float y;

#pragma region Member function
				/**
				 * \brief Get the norm of the current \c Vec2
				 * \return The norm of the \c Vec2
				 */
				float Norm() const;

				/**
				 * \brief Get the squared norm of the current \c Vec2
				 * \return The squared norm of the \c Vec2
				 */
				float NormSquared() const;

				/**
				 * \brief Normalize this \c Vec2
				 */
				void Normalize();

				/**
				 * \brief Get the Normalized vector of this \c Vec2
				 * \return The normalized \c Vec2
				 */
				Vec2 Normalized() const;

				/**
				 * \brief Get the Inner Product of this \c Vec2 and the parameter
				 * \param a _IN_ The other \c Vec2
				 * \return The new \c Vec2
				 */
				Vec2 InnerProduct( const Vec2& a ) const;

				/**
				 * \brief Get the distance to the parameter
				 * \param to _IN_ The target position
				 * \return The distance to the parameter
				 */
				float DistanceTo( const Vec2& to ) const;

				/**
				 * \brief Gets if this \c Vec2 is Zero
				 * \return True if zero, false otherwise
				 */
				bool IsZero() const;
#pragma endregion

#pragma region Operators
				Vec2 operator*( const float value ) const;
				Vec2 operator+( const Vec2& a ) const;
				Vec2 operator-( const Vec2& a ) const;
				bool operator==( const Vec2& a ) const;
				bool operator!=( const Vec2& a ) const;
				Vec2& operator+=( const Vec2& a );
				Vec2& operator-=( const Vec2& a );
				Vec2& operator*=( const float value );
				const float& operator[]( const int index ) const;
				float& operator[]( const int index );

#pragma endregion

#pragma region Static functions
				/**
				 * \brief Lerps \c Vec2 between parameters
				 * \param a _IN_ First value
				 * \param b _IN_ Second value
				 * \param t _IN_ Ratio from \p a to \p b
				 * \return The lerped \c Vec2
				 */
				static Vec2 Lerp( const Vec2& a, const Vec2& b, float t );

				/**
				 * \brief Executes a scalar product on two \c Vec2
				 * \param a _IN_ First \c Vec2
				 * \param b _IN_ Second \c Vec2
				 * \return The result of the product
				 */
				static float ScalarProduct( const Vec2& a, const Vec2& b );

				static Vec2 Right();
				static Vec2 Up();
#pragma endregion
			};
		}
	}
}
