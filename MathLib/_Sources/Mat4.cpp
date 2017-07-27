#include"MathLib//Mat4.h"

#include "MathLib/Mat3.h"
#include "MathLib/Vec4.h"
#include "MathLib/Vec3.h"
#include "MathLib/MathUtils.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			Mat4 Mat4::Identity()
			{
				return {
					1.f, 0.f, 0.f, 0.f,
					0.f, 1.f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f,
				};
			}

			Mat4::Mat4()
			{
				for ( size_t i = 0; i < 16; ++i )
					m_values[i] = 0.0f;
			}

			Mat4::Mat4( const float& m0, const float& m1, const float& m2, const float& m3
			            , const float& m4, const float& m5, const float& m6, const float& m7
			            , const float& m8, const float& m9, const float& m10, const float& m11
			            , const float& m12, const float& m13, const float& m14, const float& m15 )
			{
				m_values[0] = m0;
				m_values[1] = m1;
				m_values[2] = m2;
				m_values[3] = m3;
				m_values[4] = m4;
				m_values[5] = m5;
				m_values[6] = m6;
				m_values[7] = m7;
				m_values[8] = m8;
				m_values[9] = m9;
				m_values[10] = m10;
				m_values[11] = m11;
				m_values[12] = m12;
				m_values[13] = m13;
				m_values[14] = m14;
				m_values[15] = m15;
			}

			Mat4::Mat4( const Mat4& mat )
			{
				for ( size_t i = 0; i < 16; ++i )
					m_values[i] = mat.m_values[i];
			}

			Mat4::Mat4( const Mat3& mat )
			{
				m_values[0] = mat.m_values[0];
				m_values[1] = mat.m_values[1];
				m_values[2] = mat.m_values[2];
				m_values[3] = 0;

				m_values[4] = mat.m_values[3];
				m_values[5] = mat.m_values[4];
				m_values[6] = mat.m_values[5];
				m_values[7] = 0;

				m_values[8] = mat.m_values[6];
				m_values[9] = mat.m_values[7];
				m_values[10] = mat.m_values[8];

				m_values[11] = 0;
				m_values[12] = 0;
				m_values[13] = 0;
				m_values[14] = 0;

				m_values[15] = 1.0f;
			}

			Mat4::Mat4( const float* tab )
			{
				for ( size_t i = 0; i < 16; ++i )
					m_values[i] = tab[i];
			}

#pragma region Invert
			void Mat4::Invert()
			{
				Mat4 matrix4 = this->Inverted();

				for ( size_t i = 0; i < 16; i++ )
					m_values[i] = matrix4.m_values[i];
			}

			Mat4 Mat4::Inverted() const
			{
				float inv[16];

				inv[0] = m_values[5] * m_values[10] * m_values[15] -
						m_values[5] * m_values[11] * m_values[14] -
						m_values[9] * m_values[6] * m_values[15] +
						m_values[9] * m_values[7] * m_values[14] +
						m_values[13] * m_values[6] * m_values[11] -
						m_values[13] * m_values[7] * m_values[10];

				inv[4] = -m_values[4] * m_values[10] * m_values[15] +
						m_values[4] * m_values[11] * m_values[14] +
						m_values[8] * m_values[6] * m_values[15] -
						m_values[8] * m_values[7] * m_values[14] -
						m_values[12] * m_values[6] * m_values[11] +
						m_values[12] * m_values[7] * m_values[10];

				inv[8] = m_values[4] * m_values[9] * m_values[15] -
						m_values[4] * m_values[11] * m_values[13] -
						m_values[8] * m_values[5] * m_values[15] +
						m_values[8] * m_values[7] * m_values[13] +
						m_values[12] * m_values[5] * m_values[11] -
						m_values[12] * m_values[7] * m_values[9];

				inv[12] = -m_values[4] * m_values[9] * m_values[14] +
						m_values[4] * m_values[10] * m_values[13] +
						m_values[8] * m_values[5] * m_values[14] -
						m_values[8] * m_values[6] * m_values[13] -
						m_values[12] * m_values[5] * m_values[10] +
						m_values[12] * m_values[6] * m_values[9];

				inv[1] = -m_values[1] * m_values[10] * m_values[15] +
						m_values[1] * m_values[11] * m_values[14] +
						m_values[9] * m_values[2] * m_values[15] -
						m_values[9] * m_values[3] * m_values[14] -
						m_values[13] * m_values[2] * m_values[11] +
						m_values[13] * m_values[3] * m_values[10];

				inv[5] = m_values[0] * m_values[10] * m_values[15] -
						m_values[0] * m_values[11] * m_values[14] -
						m_values[8] * m_values[2] * m_values[15] +
						m_values[8] * m_values[3] * m_values[14] +
						m_values[12] * m_values[2] * m_values[11] -
						m_values[12] * m_values[3] * m_values[10];

				inv[9] = -m_values[0] * m_values[9] * m_values[15] +
						m_values[0] * m_values[11] * m_values[13] +
						m_values[8] * m_values[1] * m_values[15] -
						m_values[8] * m_values[3] * m_values[13] -
						m_values[12] * m_values[1] * m_values[11] +
						m_values[12] * m_values[3] * m_values[9];

				inv[13] = m_values[0] * m_values[9] * m_values[14] -
						m_values[0] * m_values[10] * m_values[13] -
						m_values[8] * m_values[1] * m_values[14] +
						m_values[8] * m_values[2] * m_values[13] +
						m_values[12] * m_values[1] * m_values[10] -
						m_values[12] * m_values[2] * m_values[9];

				inv[2] = m_values[1] * m_values[6] * m_values[15] -
						m_values[1] * m_values[7] * m_values[14] -
						m_values[5] * m_values[2] * m_values[15] +
						m_values[5] * m_values[3] * m_values[14] +
						m_values[13] * m_values[2] * m_values[7] -
						m_values[13] * m_values[3] * m_values[6];

				inv[6] = -m_values[0] * m_values[6] * m_values[15] +
						m_values[0] * m_values[7] * m_values[14] +
						m_values[4] * m_values[2] * m_values[15] -
						m_values[4] * m_values[3] * m_values[14] -
						m_values[12] * m_values[2] * m_values[7] +
						m_values[12] * m_values[3] * m_values[6];

				inv[10] = m_values[0] * m_values[5] * m_values[15] -
						m_values[0] * m_values[7] * m_values[13] -
						m_values[4] * m_values[1] * m_values[15] +
						m_values[4] * m_values[3] * m_values[13] +
						m_values[12] * m_values[1] * m_values[7] -
						m_values[12] * m_values[3] * m_values[5];

				inv[14] = -m_values[0] * m_values[5] * m_values[14] +
						m_values[0] * m_values[6] * m_values[13] +
						m_values[4] * m_values[1] * m_values[14] -
						m_values[4] * m_values[2] * m_values[13] -
						m_values[12] * m_values[1] * m_values[6] +
						m_values[12] * m_values[2] * m_values[5];

				inv[3] = -m_values[1] * m_values[6] * m_values[11] +
						m_values[1] * m_values[7] * m_values[10] +
						m_values[5] * m_values[2] * m_values[11] -
						m_values[5] * m_values[3] * m_values[10] -
						m_values[9] * m_values[2] * m_values[7] +
						m_values[9] * m_values[3] * m_values[6];

				inv[7] = m_values[0] * m_values[6] * m_values[11] -
						m_values[0] * m_values[7] * m_values[10] -
						m_values[4] * m_values[2] * m_values[11] +
						m_values[4] * m_values[3] * m_values[10] +
						m_values[8] * m_values[2] * m_values[7] -
						m_values[8] * m_values[3] * m_values[6];

				inv[11] = -m_values[0] * m_values[5] * m_values[11] +
						m_values[0] * m_values[7] * m_values[9] +
						m_values[4] * m_values[1] * m_values[11] -
						m_values[4] * m_values[3] * m_values[9] -
						m_values[8] * m_values[1] * m_values[7] +
						m_values[8] * m_values[3] * m_values[5];

				inv[15] = m_values[0] * m_values[5] * m_values[10] -
						m_values[0] * m_values[6] * m_values[9] -
						m_values[4] * m_values[1] * m_values[10] +
						m_values[4] * m_values[2] * m_values[9] +
						m_values[8] * m_values[1] * m_values[6] -
						m_values[8] * m_values[2] * m_values[5];

				float det = m_values[0] * inv[0] + m_values[1] * inv[4] + m_values[2] * inv[8] + m_values[3] * inv[12];

				if ( det == 0.0f )
					return Identity();

				det = 1.0f / det;

				Mat4 matrix4 = Mat4();

				for ( size_t i = 0; i < 16; i++ )
					matrix4.m_values[i] = inv[i] * det;

				return matrix4;
			}
#pragma endregion

#pragma region Transpose
			void Mat4::Transpose()
			{
				std::swap( m_values[1], m_values[4] );
				std::swap( m_values[2], m_values[8] );
				std::swap( m_values[3], m_values[12] );

				//matrix4.m[5] = m[5];
				std::swap( m_values[6], m_values[9] );
				std::swap( m_values[7], m_values[13] );

				//matrix4.m[10] = m[10];
				std::swap( m_values[11], m_values[14] );
				//matrix4.m[15] = m[15];
			}

			// Mat4 A = B.Transposed()
			Mat4 Mat4::Transposed() const
			{
				Mat4 matrix4 = Mat4( *this );
				matrix4.Transpose();

				return matrix4;
			}
#pragma endregion

			Vec3 Mat4::CreateVec3TranslationFromMatrix( const Mat4& matBase )
			{
				Vec3 vecTranslation =
				{
					matBase.m_values[3], matBase.m_values[7], matBase.m_values[11],
				};

				return vecTranslation;
			}

			bool Mat4::IsIdentity() const
			{
				if ( m_values[0] != 1.0f
					|| m_values[1] != 0.0f
					|| m_values[2] != 0.0f
					|| m_values[3] != 0.0f
					|| m_values[4] != 0.0f
					|| m_values[5] != 1.0f
					|| m_values[6] != 0.0f
					|| m_values[7] != 0.0f
					|| m_values[8] != 0.0f
					|| m_values[9] != 0.0f
					|| m_values[10] != 1.0f
					|| m_values[11] != 0.0f
					|| m_values[12] != 0.0f
					|| m_values[13] != 0.0f
					|| m_values[14] != 0.0f
					|| m_values[15] != 1.0f )
					return false;
				return true;
			}

			bool Mat4::IsOrthogonal() const
			{
				for ( int i = 0; i < 3; i++ )
				{
					const Vec4 current = GetColumn( i );
					if ( current.Norm() != 1.0f )
						return false;
					for ( int j = i + 1; j < 4; j++ )
					{
						if ( Vec4::ScalarProduct( current, GetColumn( j ) ) != 0 )
							return false;
					}
				}
				const Vec4 lastVec = GetColumn( 3 );
				if ( lastVec.Norm() != 1.0f )
					return false;

				return true;
			}

			Vec4 Mat4::GetRow( const int row ) const
			{
				if ( row < 0 || row > 3 )
					return Vec4();
				return Vec4( m[row][0], m[row][1], m[row][2], m[row][3] );
			}

			Vec4 Mat4::GetColumn( const int column ) const
			{
				if ( column < 0 || column > 3 )
					return Vec4();
				return Vec4( m[0][column], m[1][column], m[2][column], m[3][column] );
			}

			Vec3 Mat4::GetTranslation( const bool transposed ) const
			{
				return CreateVec3TranslationFromMatrix( transposed ? this->Transposed() : *this );
			}

			Vec3 Mat4::ExtractEulerAngles( const bool transposed ) const
			{
				Mat4 copy = *this;
				if ( transposed )
					copy.Transpose();

				Vec3 result;
				result.x = atan2( -copy.GetRow( 1 ).z, copy.GetRow( 2 ).z );
				float cosYangle = sqrt( pow( copy.GetRow( 0 ).x, 2 ) + pow( copy.GetRow( 0 ).y, 2 ) );
				result.y = atan2( copy.GetRow( 0 ).z, cosYangle );
				float sinXangle = sin( result.x );
				float cosXangle = cos( result.z );
				result.z = atan2( cosXangle * copy.GetRow( 1 ).x + sinXangle * copy.GetRow( 2 ).x,
				                  cosXangle * copy.GetRow( 1 ).y + sinXangle * copy.GetRow( 2 ).y );

				result.x = Utils::ToDegree( result.x );
				result.y = Utils::ToDegree( result.y );
				result.z = Utils::ToDegree( result.z );
				return result;
			}

			Vec3 Mat4::ExtractEulerAngles1( const bool transposed ) const
			{
				Mat4 copy = *this;
				if ( transposed )
					copy.Transpose();

				return Mat3( copy ).ExtractEulerAngles();
			}

			Vec3 Mat4::ExtractScale( const bool transposed ) const
			{
				Mat4 copy = *this;
				if ( transposed )
					copy.Transpose();

				Vec3 result;
				result.x = Vec3( copy.GetColumn( 0 ) ).Norm();
				result.y = Vec3( copy.GetColumn( 1 ) ).Norm();
				result.z = Vec3( copy.GetColumn( 2 ) ).Norm();
				return result;
			}

#pragma region Operators

			Mat4 Mat4::operator*( const Mat4& matr ) const
			{
				Mat4 matrix4 = Mat4();

				for ( size_t column = 0; column < 4; column++ )
				{
					for ( size_t line = 0; line < 4; line++ )
					{
						float sum = 0;

						sum += m_values[line + 0] * matr.m_values[column * 4 + 0];
						sum += m_values[line + 4] * matr.m_values[column * 4 + 1];
						sum += m_values[line + 8] * matr.m_values[column * 4 + 2];
						sum += m_values[line + 12] * matr.m_values[column * 4 + 3];

						matrix4.m_values[line + column * 4] = sum;
					}
				}

				return matrix4;
			}

			Mat4 Mat4::operator*( const float& val ) const
			{
				Mat4 result( *this );
				const size_t maxSize = 16;
				for ( size_t index = 0; index < maxSize; ++index )
					result.m_values[index] *= val;
				return result;
			}

			Mat4& Mat4::operator*=( const float& val )
			{
				( *this ) = ( *this ) * val;
				return ( *this );
			}

			Mat4& Mat4::operator*=( const Mat4& matr )
			{
				( *this ) = ( *this ) * matr;
				return ( *this );
			}

			Vec4 Mat4::operator*( const Vec4& vecr ) const
			{
				Vec4 matrix4 = Vec4(
					m_values[0] * vecr.x + m_values[1] * vecr.y + m_values[2] * vecr.z + m_values[3] * vecr.w,
					m_values[4] * vecr.x + m_values[5] * vecr.y + m_values[6] * vecr.z + m_values[7] * vecr.w,
					m_values[8] * vecr.x + m_values[9] * vecr.y + m_values[10] * vecr.z + m_values[11] * vecr.w,
					m_values[12] * vecr.x + m_values[13] * vecr.y + m_values[14] * vecr.z + m_values[15] * vecr.w
				);

				return matrix4;
			}

			Vec3 Mat4::operator*( const Vec3& vecr ) const
			{
				Vec4 vec4 = *this * Vec4( vecr );
				return Vec3( vec4.x, vec4.y, vec4.z );
			}

			bool Mat4::operator==( const Mat4& matr ) const
			{
				bool isEqual = true;
				for ( int index = 0; index < 16 || !isEqual; ++index )
					m_values[index] > matr.GetRawValueAt( index ) - Utils::epsilon && m_values[index] < matr.GetRawValueAt( index ) + Utils::epsilon ? isEqual = false : isEqual = true;
				return isEqual;
			}

			bool Mat4::operator!=( const Mat4& matr ) const
			{
				return !( *this == matr );
			}

			Vec4& Mat4::operator[]( const int row )
			{
				return *( reinterpret_cast<Vec4 *>(&m_values[row * 4]) );
				//return Vec4( m_values[row * 4], m_values[row * 4 + 1], m_values[row * 4 + 2], m_values[row * 4 + 3] );
			}

			const Vec4& Mat4::operator[]( const int row ) const
			{
				return *( reinterpret_cast<const Vec4 *>(&m_values[row * 4]) );
				//return Vec4( m_values[row * 4], m_values[row * 4 + 1], m_values[row * 4 + 2], m_values[row * 4 + 3] );
			}

			Mat4& Mat4::operator=( const Mat4& matr )
			{
				m_values[0] = matr.m_values[0];
				m_values[1] = matr.m_values[1];
				m_values[2] = matr.m_values[2];
				m_values[3] = matr.m_values[3];
				m_values[4] = matr.m_values[4];
				m_values[5] = matr.m_values[5];
				m_values[6] = matr.m_values[6];
				m_values[7] = matr.m_values[7];
				m_values[8] = matr.m_values[8];
				m_values[9] = matr.m_values[9];
				m_values[10] = matr.m_values[10];
				m_values[11] = matr.m_values[11];
				m_values[12] = matr.m_values[12];
				m_values[13] = matr.m_values[13];
				m_values[14] = matr.m_values[14];
				m_values[15] = matr.m_values[15];

				return *this;
			}

			Mat4& Mat4::operator+=( const Mat4& matr )
			{
				m_values[0] += matr.m_values[0];
				m_values[1] += matr.m_values[1];
				m_values[2] += matr.m_values[2];
				m_values[3] += matr.m_values[3];
				m_values[4] += matr.m_values[4];
				m_values[5] += matr.m_values[5];
				m_values[6] += matr.m_values[6];
				m_values[7] += matr.m_values[7];
				m_values[8] += matr.m_values[8];
				m_values[9] += matr.m_values[9];
				m_values[10] += matr.m_values[10];
				m_values[11] += matr.m_values[11];
				m_values[12] += matr.m_values[12];
				m_values[13] += matr.m_values[13];
				m_values[14] += matr.m_values[14];
				m_values[15] += matr.m_values[15];
				return *this;
			}

#pragma endregion

#pragma region Static Functions

#pragma region Rotation functions
			Mat4 Mat4::CreateRotZ( const float& angle )
			{
				const float rad = Utils::ToRadian( angle );

				Mat4 matRotZ =
				{
					cosf( rad ), -sinf( rad ), 0.0f, 0.0f,
					sinf( rad ), cosf( rad ), 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,
				};

				return matRotZ;
			}

			Mat4 Mat4::CreateRotY( const float& angle )
			{
				const float rad = Utils::ToRadian( angle );

				Mat4 matRotY =
				{
					cosf( rad ), 0.0f, sinf( rad ), 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					-sinf( rad ), 0.0f, cosf( rad ), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,
				};

				return matRotY;
			}

			Mat4 Mat4::CreateRotX( const float& angle )
			{
				const float rad = Utils::ToRadian( angle );

				Mat4 matRotX =
				{
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, cosf( rad ), -sinf( rad ), 0.0f,
					0.0f, sinf( rad ), cosf( rad ), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,
				};

				return matRotX;
			}

			Mat4 Mat4::CreateRotation( const float& x, const float& y, const float& z )
			{
				return CreateRotZ( z ) * CreateRotY( y ) * CreateRotX( x );
			}

			Mat4 Mat4::CreateRotation( const Vec3& rotation )
			{
				return CreateRotation( rotation.x, rotation.y, rotation.z );
			}

#pragma endregion

			Mat4 Mat4::CreateTranslation( const float& x, const float& y, const float& z )
			{
				Mat4 matTranslate =
				{
					1.0f, 0.0f, 0.0f, x,
					0.0f, 1.0f, 0.0f, y,
					0.0f, 0.0f, 1.0f, z,
					0.0f, 0.0f, 0.0f, 1.0f,
				};

				return matTranslate;
			}

			Mat4 Mat4::CreateTranslation( const Vec3& position )
			{
				return CreateTranslation( position.x, position.y, position.z );
			}

			Mat4 Mat4::CreateTransposedTranslation( const float& x, const float& y, const float& z )
			{
				Mat4 matTranslate =
				{
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x, y, z, 1.0f,
				};

				return matTranslate;
			}

			Mat4 Mat4::CreateTransposedTranslation( const Vec3& position )
			{
				return CreateTransposedTranslation( position.x, position.y, position.z );
			}

			Mat4 Mat4::CreateScale( const float& x, const float& y, const float& z )
			{
				Mat4 matScale =
				{
					x, 0.0f, 0.0f, 0.0f,
					0.0f, y, 0.0f, 0.0f,
					0.0f, 0.0f, z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,
				};

				return matScale;
			}

			Mat4 Mat4::CreateScale( const Vec3& scale )
			{
				return CreateScale( scale.x, scale.y, scale.z );
			}

			Mat4 Mat4::CreateTransform( const float& tx, const float& ty, const float& tz,
			                            const float& rx, const float& ry, const float& rz,
			                            const float& sx, const float& sy, const float& sz )
			{
				return CreateScale( sx, sy, sz ) * CreateRotation( Vec3( rx, ry, rz ) ) * CreateTranslation( tx, ty, tz );
			}

			Mat4 Mat4::CreateTransform( const Vec3& translation, const Vec3& rotation, const Vec3& scale )
			{
				return CreateTransform( translation.x, translation.y, translation.z,
				                        rotation.x, rotation.y, rotation.z,
				                        scale.x, scale.y, scale.z );
			}

			Mat4 Mat4::Negate( const Mat4& matBase )
			{
				Mat4 matNegate =
				{
					-matBase.m_values[0], -matBase.m_values[1], -matBase.m_values[2], -matBase.m_values[3],
					-matBase.m_values[4], -matBase.m_values[5], -matBase.m_values[6], -matBase.m_values[7],
					-matBase.m_values[8], -matBase.m_values[9], -matBase.m_values[10], -matBase.m_values[11],
					-matBase.m_values[12], -matBase.m_values[13], -matBase.m_values[14], -matBase.m_values[15],
				};

				return matNegate;
			}

			Mat4 Mat4::NegateTranslate( const Mat4& matBase )
			{
				Mat4 matNegate =
				{
					matBase.m_values[0], matBase.m_values[1], matBase.m_values[2], -matBase.m_values[3],
					matBase.m_values[4], matBase.m_values[5], matBase.m_values[6], -matBase.m_values[7],
					matBase.m_values[8], matBase.m_values[9], matBase.m_values[10], -matBase.m_values[11],
					matBase.m_values[12], matBase.m_values[13], matBase.m_values[14], matBase.m_values[15],
				};

				return matNegate;
			}
#pragma endregion

			Mat4 Mat4::ProjMatrix( const float& width, const float& height, const float& degreeFov, const float& nearDistance, const float& farDistance )
			{
				// Projection perspective
				const float aspectRatio = width / height; // Pour que les pixels soient carres
				const float fov = Utils::ToRadian( degreeFov );
				const float d = 1.0f / tan( fov / 2.0f ); // Distance focale (oeil -> plan de projection)
				const float range = 1.0f / ( nearDistance - farDistance ); // normalisation en NDC de la profondeur

				Mat4 projectionMatrix = Mat4
				( d / aspectRatio, 0.0f, 0.0f, 0.0f
				  , 0.0f, d, 0.0f, 0.0f
				  , 0.0f, 0.0f, ( nearDistance + farDistance ) * range, ( nearDistance * farDistance * 2.0f ) * range
				  , 0.0f, 0.0f, -1.0f, 0.0f );

				return projectionMatrix;
			}

			Mat4 Mat4::LookAt( const Vec3& origin, const Vec3& target, const Vec3 worldUp )
			{
				const Vec3 zaxis = ( target - origin ).Normalized();
				const Vec3 xaxis = Vec3::CrossProduct( worldUp, zaxis ).Normalized();
				const Vec3 yaxis = Vec3::CrossProduct( zaxis, xaxis ).Normalized();
				return Mat4(
					xaxis.x, yaxis.x, zaxis.x, -Vec3::ScalarProduct( xaxis, origin ),
					xaxis.y, yaxis.y, zaxis.y, -Vec3::ScalarProduct( yaxis, origin ),
					xaxis.z, yaxis.z, zaxis.z, -Vec3::ScalarProduct( zaxis, origin ),
					0.0f, 0.0f, 0.0f, 1.0f
				);
			}

			Mat4 Mat4::OrthoProj( const float& left, const float& right, const float& bottom, const float& top, const float& zNear, const float& zFar )
			{
				Mat4 result = Identity();
				result[0][0] = 2.0f / ( right - left );
				result[1][1] = 2.0f / ( top - bottom );
				result[2][2] = -2.0f / ( zFar - zNear );
				result[3][0] = -( right + left ) / ( right - left );
				result[3][1] = -( top + bottom ) / ( top - bottom );
				result[3][2] = -( zFar + zNear ) / ( zFar - zNear );
				return result;
			}

			Mat4 Mat4::OrthoProj( const int width, const int height )
			{
				return OrthoProj( 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f );
			}
		}
	}
}
