#include "MathLib/Mat3.h"
#include "MathLib/Mat4.h"
#include "MathLib/Vec3.h"
#include "MathLib/MathUtils.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			Mat3::Mat3()
			{
				for ( size_t i = 0; i < 9; ++i )
					m_values[i] = 0.0f;
			}

			Mat3::Mat3( const Mat3& mat )
			{
				for ( size_t i = 0; i < 9; ++i )
					m_values[i] = mat.m_values[i];
			}

			Mat3::Mat3( const Mat4& mat )
			{
				m_values[0] = mat.GetRawValueAt( 0 );
				m_values[1] = mat.GetRawValueAt( 1 );
				m_values[2] = mat.GetRawValueAt( 2 );

				m_values[3] = mat.GetRawValueAt( 4 );
				m_values[4] = mat.GetRawValueAt( 5 );
				m_values[5] = mat.GetRawValueAt( 6 );

				m_values[6] = mat.GetRawValueAt( 8 );
				m_values[7] = mat.GetRawValueAt( 9 );
				m_values[8] = mat.GetRawValueAt( 10 );
			}

			Mat3::Mat3( const float* tab )
			{
				for ( size_t i = 0; i < 9; ++i )
					m_values[i] = tab[i];
			}

			Mat3::Mat3( const float& m0, const float& m1, const float& m2
			            , const float& m3, const float& m4, const float& m5
			            , const float& m6, const float& m7, const float& m8 )
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
			}

#pragma region Transpose
			void Mat3::Transpose()
			{
				std::swap( m_values[1], m_values[3] );
				std::swap( m_values[2], m_values[6] );
				std::swap( m_values[5], m_values[7] );
			}

			Mat3 Mat3::Transposed() const
			{
				Mat3 matrix3 = Mat3( *this );
				matrix3.Transpose();

				return matrix3;
			}
#pragma endregion

			Mat3 Mat3::Identity()
			{
				Mat3 id = Mat3();

				id.m_values[0] = 1.0f;
				id.m_values[4] = 1.0f;
				id.m_values[8] = 1.0f;

				return id;
			}

			Mat3 Mat3::operator*( const Mat3& matr )
			{
				Mat3 matrix3 = Mat3();

				for ( size_t line = 0; line < 3; line++ )
				{
					for ( size_t column = 0; column < 3; column++ )
					{
						float sum = 0;

						sum += m_values[( line * 3 ) + 0] * matr.m_values[column + 0];
						sum += m_values[( line * 3 ) + 1] * matr.m_values[column + 3];
						sum += m_values[( line * 3 ) + 2] * matr.m_values[column + 6];

						matrix3.m_values[line * 3 + column] = sum;
					}
				}

				return matrix3;
			}

			bool Mat3::operator==( const Mat3& mat ) const
			{
				const size_t maxSize = 9;
				for ( int index = 0; index < maxSize; ++index )
					if ( mat.m_values[index] != m_values[index] )
						return false;
				return true;
			}

			bool Mat3::operator!=( const Mat3& mat ) const
			{
				return !( *this == mat );
			}

#pragma region Rotation functions
			Mat3 Mat3::CreateRotZ( const float& angle )
			{
				const float rad = Utils::ToRadian( angle );

				Mat3 matRotZ =
				{
					cosf( rad ), -sinf( rad ), 0.0f,
					sinf( rad ), cosf( rad ), 0.0f,
					0.0f, 0.0f, 1.0f
				};

				return matRotZ;
			}

			Mat3 Mat3::CreateRotY( const float& angle )
			{
				const float rad = Utils::ToRadian( angle );

				Mat3 matRotY =
				{
					cosf( rad ), 0.0f, sinf( rad ),
					0.0f, 1.0f, 0.0f,
					-sinf( rad ), 0.0f, cosf( rad )
				};

				return matRotY;
			}

			Mat3 Mat3::CreateRotX( const float& angle )
			{
				const float rad = Utils::ToRadian( angle );

				Mat3 matRotX =
				{
					1.0f, 0.0f, 0.0f,
					0.0f, cosf( rad ), -sinf( rad ),
					0.0f, sinf( rad ), cosf( rad )
				};

				return matRotX;
			}

			Mat3 Mat3::CreateRotation( const float& x, const float& y, const float& z )
			{
				return CreateRotZ( z ) * CreateRotY( y ) * CreateRotX( x );
			}

			Mat3 Mat3::CreateRotation( const Vec3& rotation )
			{
				return CreateRotation( rotation.x, rotation.y, rotation.z );
			}

			Vec3 Mat3::ExtractEulerAngles()
			{
				Vec3 result;
				// Assuming the angles are in radians.
				/*if ( m_values[3] > 0.998 )
				{ // singularity at north pole
					result.y = atan2( m_values[2], m_values[8] );
					result.x = 0;
					result.z = 0;
				}
				else if ( m_values[3] < -0.998 )
				{ // singularity at south pole
					result.y = atan2( m_values[2], m_values[8] );
					result.x = 0;
					result.z = 0;
					return result;
				}
				else
				{
					result.y = atan2( -m_values[6], m_values[0] );
					result.z = atan2( -m_values[5], m_values[4] );
					result.x = asin( m_values[3] );
				}*/
				result.y = Utils::ToDegree( acos( m_values[8] ) );
				result.x = Utils::ToDegree( atan2( m_values[7], -m_values[6] ) );
				result.z = Utils::ToDegree( atan2( m_values[5], m_values[2] ) );
				return result;
			}
#pragma endregion
		}
	}
}
