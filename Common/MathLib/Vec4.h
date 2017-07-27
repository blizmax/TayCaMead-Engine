#pragma once
#include "MathLibDefine.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec3;

			struct MATHLIB_DLL_EXPORT Vec4
			{
				Vec4();
				Vec4( const Vec4& old );
				explicit Vec4( const Vec3& old, const float& _w = 1.0f );
				Vec4( const float& _x, const float& _y, const float& _z, const float& _w );

				float x;
				float y;
				float z;
				float w;

#pragma region Member functions
				/**
				 * \brief Get the norm of the current \c Vec4
				 * \return The norm of the \c Vec4
				 */
				float Norm() const;

				/**
				 * \brief Get the squared norm of the current \c Vec4
				 * \return The squared norm of the \c Vec4
				 */
				float NormSquared() const;

				/**
				 * \brief Normalize this \c Vec4
				 */
				void Normalize();

				/**
				 * \brief Get the Normalized vector of this \c Vec4
				 * \return The normalized \c Vec4
				 */
				Vec4 Normalized() const;

				/**
				 * \brief Executes an Inner Product between this \c Vec4 and another one
				 * \param a _IN_ Other \c Vec4
				 * \return The result of the product
				 */
				Vec4 InnerProduct( const Vec4& a ) const;

				/**
				 * \brief Get the distance to the parameter
				 * \param to _IN_ The target position
				 * \return The distance to the parameter
				 */
				float DistanceTo( const Vec4& to ) const;

				/**
				 * \brief Gets if this \c Vec4 is Zero
				 * \return True if zero, false otherwise
				 */
				bool IsZero() const;

				/**
				 * \brief Get the \c Vec3 coming from this \c Vec4
				 * \return The new \c Vec3
				 */
				Vec3 ToVec3() const;
#pragma endregion

#pragma region Operators
				Vec4 operator*( const float& value ) const;
				Vec4 operator+( const Vec4& a ) const;
				Vec4 operator-( const Vec4& a ) const;
				bool operator==( const Vec4& a ) const;
				bool operator!=( const Vec4& a ) const;
				Vec4& operator+=( const Vec4& a );
				Vec4& operator-=( const Vec4& a );
				Vec4& operator*=( const float& value );
				const float& operator[]( const int index ) const;
				float& operator[]( const int index );
#pragma endregion

#pragma region Static functions
				static Vec4 Lerp( const Vec4& a, const Vec4& b, const float& t );
				static float ScalarProduct( const Vec4& a, const Vec4& b );
#pragma endregion
			};
		}
	}
}
