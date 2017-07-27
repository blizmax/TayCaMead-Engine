#pragma once
#include "MathLibDefine.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec4;

			struct MATHLIB_DLL_EXPORT Vec3
			{
				Vec3();
				Vec3( const Vec3& old );
				explicit Vec3( const Vec4& old );
				Vec3( const float& _x, const float& _y, const float& _z );

				float x;
				float y;
				float z;

				static Vec3 Forward();
				static Vec3 Right();
				static Vec3 Up();

#pragma region Member functions
				/**
				 * \brief Get the norm of the current \c Vec3
				 * \return The norm of the \c Vec3
				 */
				float Norm() const;

				/**
				 * \brief Get the squared norm of the current \c Vec3
				 * \return The squared norm of the \c Vec3
				 */
				float NormSquared() const;

				/**
				 * \brief Normalize this \c Vec3
				 */
				void Normalize();

				/**
				 * \brief Get the Normalized vector of this \c Vec3
				 * \return The normalized \c Vec3
				 */
				Vec3 Normalized() const;

				/**
				 * \brief Executes an Inner Product between this \c Vec3 and another one
				 * \param a _IN_ Other \c Vec3
				 * \return The result of the product
				 */
				Vec3 InnerProduct( const Vec3& a ) const;

				/**
				 * \brief Get the distance to the parameter
				 * \param to _IN_ The target position
				 * \return The distance to the parameter
				 */
				float DistanceTo( const Vec3& to ) const;
				float DistanceSquaredTo( const Vec3& to ) const;

				/**
				 * \brief Get the radius of the Box formed using Width/Height/Length
				 * \return The Box radius
				 */
				float GetBoxRadius() const;

				/**
				 * \brief Gets if this \c Vec3 is Zero
				 * \return True if zero, false otherwise
				 */
				bool IsZero() const;
#pragma endregion

#pragma region Operators
				Vec3 operator*( const float& value ) const;
				Vec3 operator/( const float& value ) const;
				Vec3 operator+( const Vec3& a ) const;
				Vec3 operator-( const Vec3& a ) const;
				bool operator==( const Vec3& a ) const;
				bool operator!=( const Vec3& a ) const;
				Vec3& operator+=( const Vec3& a );
				Vec3& operator-=( const Vec3& a );
				Vec3& operator*=( const float& value );
				const float& operator[]( const int index ) const;
				float& operator[]( const int index );
#pragma endregion

#pragma region Static functions
				/**
				 * \brief Lerps \c Vec3 between parameters
				 * \param a _IN_ First value
				 * \param b _IN_ Second value
				 * \param t _IN_ Ratio from \p a to \p b
				 * \return The lerped \c Vec3
				 */
				static Vec3 Lerp( const Vec3& a, const Vec3& b, const float& t );

				/**
				 * \brief Executes a scalar product on two \c Vec3
				 * \param a _IN_ First \c Vec3
				 * \param b _IN_ Second \c Vec3
				 * \return The result of the product
				 */
				static float ScalarProduct( const Vec3& a, const Vec3& b );

				/**
				 * \brief Executes a cross product on two \c Vec3
				 * \param a _IN_ First \c Vec3
				 * \param b _IN_ Second \c Vec3
				 * \return The result of the product
				 */
				static Vec3 CrossProduct( const Vec3& a, const Vec3& b );
#pragma endregion
			};
		}
	}
}
