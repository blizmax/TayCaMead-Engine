#pragma once
#include "MathLibDefine.h"
#include <array>

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Mat4;
			struct Vec3;

			struct MATHLIB_DLL_EXPORT Mat3
			{
				Mat3();
				Mat3( const Mat3& mat );
				explicit Mat3( const Mat4& mat );
				explicit Mat3( const float* tab );
				Mat3( const float& m0, const float& m1, const float& m2
				      , const float& m3, const float& m4, const float& m5
				      , const float& m6, const float& m7, const float& m8 );
				~Mat3() = default;

				union
				{
					struct
					{
						float _11, _12, _13;
						float _21, _22, _23;
						float _31, _32, _33;
					};

					float m[3][3];
					std::array<float, 9> m_values;
				};

#pragma region Member Function
				/**
				 * \brief Transposes the current Matrix
				 */
				void Transpose();

				/**
				 * \brief Get the transposed Matrix
				 * \return The transposed Matrix
				 */
				Mat3 Transposed() const;

				/**
				 * \brief Extract the Euler Angles from the Matrix
				 * \return The Euler Angles formated as a \c Vec3
				 */
				Vec3 ExtractEulerAngles();
#pragma endregion

#pragma region Operators
				Mat3 operator*( const Mat3& matr );
				bool operator==( const Mat3& mat ) const;
				bool operator!=( const Mat3& mat ) const;
#pragma endregion

#pragma region Static functions
				/**
				 * \brief Get the \c Mat3 Identity
				 * \return The identity \c Mat3
				 */
				static Mat3 Identity();

				/**
				 * \brief Creates a transform \c Mat3 for rotating Z
				 * \param angle _IN_ The angle to be used
				 * \return The new \c Mat3
				 * \note \p angle is in degree
				 */
				static Mat3 CreateRotZ( const float& angle );

				/**
				 * \brief Creates a transform \c Mat3 for rotating Y
				 * \param angle _IN_ The angle to be used
				 * \return The new \c Mat3
				 * \note \p angle is in degree
				 */
				static Mat3 CreateRotY( const float& angle );

				/**
				 * \brief Creates a transform \c Mat3 for rotating X
				 * \param angle _IN_ The angle to be used
				 * \return The new \c Mat3
				 * \note \p angle is in degree
				 */
				static Mat3 CreateRotX( const float& angle );

				/**
				 * \brief Creates a transform \c Mat3 for rotations
				 * \param x _IN_ The angle to be used for x
				 * \param y _IN_ The angle to be used for y
				 * \param z _IN_ The angle to be used for z
				 * \return The new \c Mat3
				 * \note Angles are in degree
				 */
				static Mat3 CreateRotation( const float& x, const float& y, const float& z );

				/**
				 * \brief Creates a transform \c Mat3 for rotations
				 * \param rotation _IN_ The angles to be used
				 * \return The new \c Mat3
				 * \note Angles are in degree
				 */
				static Mat3 CreateRotation( const Vec3& rotation );
#pragma endregion
			};
		}
	}
}
