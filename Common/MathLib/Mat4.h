#pragma once
#include <array>
#include "MathLibDefine.h"

#include "Vec3.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec4;
			struct Mat3;

			/**
			 * \brief Matrix 4x4 Row-major\n
			 *		  Example: Mat4[row][col]\n
			 *		  First col is axis X, second is Y, third is Z\n
			 *		  Last column is the translation
			 */
			struct MATHLIB_DLL_EXPORT Mat4
			{
			private:
				union
				{
					struct
					{
						float _11, _12, _13, _14;
						float _21, _22, _23, _24;
						float _31, _32, _33, _34;
						float _41, _42, _43, _44;
					};

					float m[4][4];
					std::array<float, 16> m_values;
				};

			public:
				Mat4();
				Mat4( const float& m0, const float& m1, const float& m2, const float& m3
				      , const float& m4, const float& m5, const float& m6, const float& m7
				      , const float& m8, const float& m9, const float& m10, const float& m11
				      , const float& m12, const float& m13, const float& m14, const float& m15 );
				Mat4( const Mat4& mat );
				explicit Mat4( const Mat3& mat );
				explicit Mat4( const float* tab );
				~Mat4() = default;

#pragma region Member Function
				/**
				 * \brief Inverts the current Matrix
				 */
				void Invert();

				/**
				 * \brief Get the inverted Matrix
				 * \return The inverted Matrix
				 */
				Mat4 Inverted() const;

				/**
				 * \brief Transposes the current Matrix
				 */
				void Transpose();

				/**
				 * \brief Get the transposed Matrix
				 * \return The transposed Matrix
				 */
				Mat4 Transposed() const;

				/**
				 * \brief Is this Matrix the identity
				 * \return True if Identity, false otherwise
				 */
				bool IsIdentity() const;

				/**
				 * \brief Is the Matrix orthogonal
				 * \return True if Orthogonal, false otherwise
				 */
				bool IsOrthogonal() const;
#pragma endregion

#pragma region Getter / Setter
				/**
				 * \brief Get raw values of the Matrix
				 * \return Raw matrix values
				 */
				inline std::array<float, 16> GetRawValues() const { return m_values; }

				/**
				 * \brief Get the raw value at an index
				 * \param index _IN_ The index of the value to be obtained
				 * \return The raw value at the \p index
				 */
				inline float GetRawValueAt( const int index ) const { return m_values[index]; }

				/**
				 * \brief Get the raw value at a row and column
				 * \param row _IN_ The row of the value to be obtained
				 * \param col _IN_ The column of the value to be obtained
				 * \return The raw value at the \p row and \p col
				 */
				inline float GetRawValueAt( const int row, const int col ) const { return m[row][col]; }

				/**
				 * \brief Get the values of a specified row index
				 * \param row _IN_ The row index to be obtained
				 * \return The \c Vec4 containing the row
				 * \note Indexes begins at 0, not 1
				 */
				Vec4 GetRow( const int row ) const;

				/**
				 * \brief Get the values of a specified column index
				 * \param column _IN_ The row index to be obtained
				 * \return The \c Vec4 containing the column
				 * \note Indexes begins at 0, not 1
				 */
				Vec4 GetColumn( const int column ) const;

				/**
				 * \brief Get the translation \c Vec3
				 * \param transposed _IN_ If needs to transpose the matrix before internally executing the function (default: false)
				 * \return The translation \c Vec3
				 */
				Vec3 GetTranslation( const bool transposed = false ) const;

				/**
				 * \brief Get the Euler Angles \c Vec3
				 * \param transposed _IN_ If needs to transpose the matrix before internally exectuting the function (default: false)
				 * \return The Euler Angles \c Vec3
				 * \note Don't use it
				 */
				Vec3 ExtractEulerAngles( const bool transposed = false ) const;

				/**
				 * \brief Get the Euler Angles \c Vec3
				 * \param transposed _IN_ If needs to transpose the matrix before internally exectuting the function (default: false)
				 * \return The Euler Angles \c Vec3
				 * \note Don't use it
				 */
				Vec3 ExtractEulerAngles1( const bool transposed = false ) const;

				/**
				 * \brief Get the Scale \c Vec3
				 * \param transposed _IN_ If needs to transpose the matrix before internally exectuting the function (default: false)
				 * \return The Scale \c Vec3
				 */
				Vec3 ExtractScale( const bool transposed = false ) const;
#pragma endregion

#pragma region Operators
				Mat4 operator*( const Mat4& matr ) const;
				Mat4 operator*( const float& val ) const;
				Mat4& operator*=( const float& val );
				Mat4& operator*=( const Mat4& matr );
				Vec4 operator*( const Vec4& vecr ) const;
				Vec3 operator*( const Vec3& vecr ) const;
				bool operator==( const Mat4& matr ) const;
				bool operator!=( const Mat4& matr ) const;
				Vec4& operator[]( const int row );
				const Vec4& operator[]( const int row ) const;
				Mat4& operator=( const Mat4& matr );
				Mat4& operator+=( const Mat4& matr );
#pragma endregion

#pragma region Static functions
				/**
				 * \brief Get the Mat4 Identity
				 * \return The identity Mat4
				 */
				static Mat4 Identity();

				/**
				 * \brief Creates a transform \c Mat4 for rotating Z
				 * \param angle _IN_ The angle to be used
				 * \return The new \c Mat4
				 * \note \p angle is in degree
				 */
				static Mat4 CreateRotZ( const float& angle );

				/**
				 * \brief Creates a transform \c Mat4 for rotating Y
				 * \param angle _IN_ The angle to be used
				 * \return The new \c Mat4
				 * \note \p angle is in degree
				 */
				static Mat4 CreateRotY( const float& angle );

				/**
				 * \brief Creates a transform \c Mat4 for rotating X
				 * \param angle _IN_ The angle to be used
				 * \return The new \c Mat4
				 * \note \p angle is in degree
				 */
				static Mat4 CreateRotX( const float& angle );

				/**
				 * \brief Creates a transform \c Mat4 for rotations
				 * \param x _IN_ The angle to be used for x
				 * \param y _IN_ The angle to be used for y
				 * \param z _IN_ The angle to be used for z
				 * \return The new \c Mat4
				 * \note Angles are in degree
				 */
				static Mat4 CreateRotation( const float& x, const float& y, const float& z );

				/**
				 * \brief Creates a transform \c Mat4 for rotations
				 * \param rotation _IN_ The angles to be used
				 * \return The new \c Mat4
				 * \note Angles are in degree
				 */
				static Mat4 CreateRotation( const Vec3& rotation );

				/**
				 * \brief Extracts the position from the this \c Mat4
				 * \param matBase _IN_ The base \c Mat4 used
				 * \return The translation \c Vec3
				 */
				static Vec3 CreateVec3TranslationFromMatrix( const Mat4& matBase );

				/**
				 * \brief Creates a transform \c Mat4 for translation
				 * \param x _IN_ The value to be used for x
				 * \param y _IN_ The value to be used for y
				 * \param z _IN_ The value to be used for z
				 * \return The new \c Mat4
				 */
				static Mat4 CreateTranslation( const float& x, const float& y, const float& z );

				/**
				 * \brief Creates a transform \c Mat4 for translation
				 * \param position _IN_ The values to be used
				 * \return The new \c Mat4
				 */
				static Mat4 CreateTranslation( const Vec3& position );

				/**
				 * \brief Creates a transform \c Mat4 for translation after an automatic transposition
				 * \param x _IN_ The value to be used for x
				 * \param y _IN_ The value to be used for y
				 * \param z _IN_ The value to be used for z
				 * \return The new \c Mat4
				 */
				static Mat4 CreateTransposedTranslation( const float& x, const float& y, const float& z );

				/**
				 * \brief Creates a transform \c Mat4 for translation after an automatic transposition
				 * \param position _IN_ The values to be used
				 * \return The new \c Mat4
				 */
				static Mat4 CreateTransposedTranslation( const Vec3& position );

				/**
				 * \brief Creates a transform \c Mat4 for scaling
				 * \param x _IN_ The value to be used for x
				 * \param y _IN_ The value to be used for y
				 * \param z _IN_ The value to be used for z
				 * \return The new \c Mat4
				 */
				static Mat4 CreateScale( const float& x, const float& y, const float& z );

				/**
				 * \brief Creates a transform \c Mat4 for scaling
				 * \param scale _IN_ The values to be used
				 * \return The new \c Mat4
				 */
				static Mat4 CreateScale( const Vec3& scale );

				/**
				 * \brief Create a transform \c Mat4
				 * \param tx _IN_ Translation X
				 * \param ty _IN_ Translation y
				 * \param tz _IN_ Translation Z
				 * \param rx _IN_ Rotation angle X (degree)
				 * \param ry _IN_ Rotation angle Y (degree)
				 * \param rz _IN_ Rotation angle Z (degree)
				 * \param sx _IN_ Scale X
				 * \param sy _IN_ Scale Y
				 * \param sz _IN_ Scale Z
				 * \return The new \c Mat4
				 */
				static Mat4 CreateTransform( const float& tx = 0.0f, const float& ty = 0.0f, const float& tz = 0.0f,
				                             const float& rx = 0.0f, const float& ry = 0.0f, const float& rz = 0.0f,
				                             const float& sx = 1.0f, const float& sy = 1.0f, const float& sz = 1.0f );

				/**
				 * \brief Create a transform \c Mat4
				 * \param translation _IN_ Translation \c Vec3
				 * \param rotation _IN_ Rotation \c Vec3
				 * \param scale _IN_ Scale \c Vec3
				 * \return The new \c Mat4
				 */
				static Mat4 CreateTransform( const Vec3& translation = { 0.0f, 0.0f, 0.0f },
				                             const Vec3& rotation = { 0.0f, 0.0f, 0.0f },
				                             const Vec3& scale = { 1.0f, 1.0f, 1.0f } );

				/**
				 * \brief Negates the whole \c Mat4
				 * \param matBase _IN_ The base \c Mat4
				 * \return The negated \c Mat4
				 */
				static Mat4 Negate( const Mat4& matBase );

				/**
				 * \brief Negates the translation of the \c Mat4 in parameter
				 * \param matBase _IN_ The base \c Mat4
				 * \return The translation-negated \c Mat4
				 */
				static Mat4 NegateTranslate( const Mat4& matBase );

				/**
				 * \brief Creates a projection matrix
				 * \param width _IN_ Width of the viewport
				 * \param height _IN_ Height of the viewport
				 * \param degreeFov _IN_ Degree Fov
				 * \param farDistance _IN_ Far distance
				 * \param nearDistance _IN_ Near distance
				 * \return The projection matrix \c Mat4
				 */
				static Mat4 ProjMatrix( const float& width, const float& height, const float& degreeFov, const float& nearDistance, const float& farDistance );

				/**
				 * \brief Creates a transform \c Mat4 to look at parameter
				 * \param origin _IN_ Origin position
				 * \param target _IN_ Target Position
				 * \param worldUp _IN_ World Up vector
				 * \return The new \c Mat4
				 */
				static Mat4 LookAt( const Vec3& origin, const Vec3& target, const Vec3 worldUp );

				/**
				 * \brief Create an Orthographic Projection Matrix 
				 * \param left _IN_ Left plane position
				 * \param right _IN_ Right plane position
				 * \param bottom _IN_ Bottom plane position
				 * \param top _IN_ Top plane position
				 * \param zNear _IN_ zNear
				 * \param zFar _IN_ zFar
				 * \return The new \c Mat4
				 */
				static Mat4 OrthoProj( const float& left, const float& right, const float& bottom, const float& top, const float& zNear, const float& zFar );

				/**
				 * \brief Create an Orhographic Projection Matrix from screen size
				 * \param width _IN_ Width of the viewport
				 * \param height _IN_ Height of the viewport
				 * \return The new \c Mat4
				 */
				static Mat4 OrthoProj( const int width, const int height );
#pragma endregion
			};
		}
	}
}
