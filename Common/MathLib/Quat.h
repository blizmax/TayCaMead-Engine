#pragma once
#include "MathLibDefine.h"
#include "Vec3.h"
#include <locale>

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Mat3;

			struct MATHLIB_DLL_EXPORT Quat
			{
			public:

				float x, y, z, w;

				Quat() { x = y = z = w = 0.f; }

				Quat( const float _x, const float _y, const float _z, const float _w ) : x( _x )
				                                                                       , y( _y )
				                                                                       , z( _z )
				                                                                       , w( _w )
				{
				}

				Quat operator+( const Quat& q1 ) const
				{
					return {
						w + q1.w,
						x + q1.x,
						y + q1.y,
						z + q1.z
					};
				}

				Quat operator+( const float value ) const
				{
					return {
						w + value,
						x + value,
						y + value,
						z + value,
					};
				}

				bool operator==( const Quat& q ) const
				{
					return w == q.w && x == q.x && y == q.y && z == q.z;
				}

				Quat operator*( const float value ) const
				{
					return { w * value,
						x * value,
						y * value,
						z * value };
				}

				Quat operator/( const float value ) const
				{
					return { w / value,
						x / value,
						y / value,
						z / value };
				}

				Quat operator*( const Quat& quat ) const
				{
					return { quat.w * w - quat.x * x - quat.y * y - quat.z * z,
						quat.w * x + quat.x * w - quat.y * z + quat.z * y,
						quat.w * y + quat.x * z + quat.y * w - quat.z * x,
						quat.w * z - quat.x * y + quat.y * x + quat.z * w };
				}

				Quat operator*( const Vec3& v ) const
				{
					Quat q( v.x, v.y, v.z, 0.0f );
					return ( *this ) * q;
				}

				/**
				 * \brief Rotate around the quaternion
				 * \param v _IN_ Point to rotate
				 * \return Point rotated
				 */
				Vec3 Rotate( const Vec3& v ) const;
				/**
				 * \brief Return the conjugate of the quaternion
				 */
				Quat Conjugated() const;
				/**
				 * \brief Convert it to euler angles
				 */
				Vec3 ToEulerAngles() const;
				/**
				 * \brief Chec if all mebers are null 
				 */
				bool IsZero() const;

				/**
				 * \brief Return norm of the quaternion
				 */
				float Norm() const;
				/**
				 * \brief Return the scalar product of two quaternions
				 */
				static float ScalarProduct( const Quat& q1, const Quat& q2 );

				static Quat Slerp( Quat q1, const Quat& q2, const float t );
				static Quat Nlerp( Quat q1, const Quat& q2, const float t );

				Vec3 ToVec3() const;
				Mat3 ToMat3() const;

				/**
				 * \brief Return the quaternion created from the angle and axis
				 * \param rotationAngle _IN_ In degree, angle of rotation
				 * \param rotationAxis _IN_ Axis
				 * \return Quaternion calculated
				 */
				static Quat AngleAxis( float rotationAngle, Vec3 rotationAxis );
				/**
				 * \brief Return the quaternion between the two vectors
				 */
				static Quat RotationBetweenVectors( Vec3 start, Vec3 dest );
				/**
				 * \brief Return the quaternion looking at
				 */
				static Quat LookAt( Vec3 direction, Vec3 desiredUp );
				/**
				 * \brief Create quaternion from euler angles
				 * \param euler _IN_ Euler angles
				 * \return Quaternion calculated
				 */
				static Quat FromEulerAngle( Vec3 euler );
			};
		}
	}
}
