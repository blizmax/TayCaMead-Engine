#include <cmath>
#include "MathLib/Quat.h"
#include "MathLib/Mat3.h"
#include "MathLib/MathUtils.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			//https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
			Vec3 Quat::Rotate( const Vec3& v ) const
			{
				Vec3 t = Vec3::CrossProduct( Vec3( x, y, z ), v ) * 2;
				return Vec3( v + ( t * w ) + Vec3::CrossProduct( Vec3( x, y, z ), t ) );
			}

			Quat Quat::Conjugated() const
			{
				Quat quat;
				quat.w = w;
				quat.x = -x;
				quat.y = -y;
				quat.z = -z;
				return quat;
			}

			Vec3 Quat::ToEulerAngles() const
			{
				float sqw = w * w;
				float sqx = x * x;
				float sqy = y * y;
				float sqz = z * z;
				float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
				float test = x * y + z * w;
				Vec3 euler;
				if ( test > 0.499f * unit )
				{ // singularity at north pole
					euler.y = 2.0f * atan2( x, w );
					euler.x = Utils::PI / 2.0f;
					euler.z = 0;
					return euler;
				}
				if ( test < -0.499 * unit )
				{ // singularity at south pole
					euler.y = -2.0f * atan2( x, w );
					euler.x = -Utils::PI / 2.0f;
					euler.z = 0;
					return euler;
				}
				euler.y = atan2( 2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw );
				euler.x = asin( 2.0f * test / unit );
				euler.z = atan2( 2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw );

				return euler;
			}

			bool Quat::IsZero() const
			{
				return x == 0.f
						&& y == 0.f
						&& z == 0.f
						&& w == 0.f;
			}

			float Quat::Norm() const
			{
				return sqrtf( w * w + x * x + y * y + z * z );
			}

			float Quat::ScalarProduct( const Quat& q1, const Quat& q2 )
			{
				return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
			}

			Quat Quat::Slerp( Quat q1, const Quat& q2, const float t )
			{
				if ( t < 0 || t >= 1.0f )
					return q1;
				float scaleProd = ScalarProduct( q1, q2 );
				if ( scaleProd < 0 )
				{
					q1 = q1 * -1.0f;
					scaleProd = ScalarProduct( q1, q2 );
				}
				float omega = acos( scaleProd );
				float oSin = sin( omega );
				if ( oSin == 0 )
					return q1;
				float w1 = ( ( sin( ( 1.0f - t ) * omega ) ) / oSin );
				float w2 = ( sin( t * omega ) / oSin );
				return q1 * w1 + q2 * w2;
			}

			Quat Quat::Nlerp( Quat q1, const Quat& q2, const float t )
			{
				if ( t < 0 || t >= 1.0f )
					return q1;
				float w1 = 1.0f - t;
				float w2 = t;
				if ( ScalarProduct( q1, q2 ) < 0 )
					q1 = q1 * -1.0f;

				Quat qt = ( q1 * w1 + q2 * w2 );
				return qt / qt.Norm();
			}

			Vec3 Quat::ToVec3() const
			{
				return Vec3( x, y, z );
			}

			Mat3 Quat::ToMat3() const
			{
				float values[] = { 1.0f - 2.0f * y * y - 2.0f * z * z, 2.0f * x * y - 2.0f * z * w, 2.0f * x * z + 2.0f * y * w,
					2.0f * x * y + 2.0f * z * w, 1.0f - 2.0f * x * x - 2.0f * z * z, 2.0f * y * z - 2.0f * x * w,
					2.0f * x * z - 2.0f * y * w, 2.0f * y * z + 2.0f * x * w, 1.0f - 2.0f * x * x - 2.0f * y * y,
				};
				return Mat3( values );
			}

			Quat Quat::AngleAxis( float rotationAngle, Vec3 rotationAxis )
			{
				float rad = Utils::ToRadian( rotationAngle );
				float s = sin( rad / 2.0f );
				Quat quat;
				quat.x = rotationAxis.x * s;
				quat.y = rotationAxis.y * s;
				quat.z = rotationAxis.z * s;
				quat.w = cos( rad / 2.0f );
				return quat;
			}

			Quat Quat::RotationBetweenVectors( Vec3 start, Vec3 dest )
			{
				start.Normalize();
				dest.Normalize();

				float cosTheta = Vec3::ScalarProduct( start, dest );
				Vec3 rotationAxis;

				if ( cosTheta < -1.0f + 0.001f )
				{
					// special case when vectors in opposite directions :
					// there is no "ideal" rotation axis
					// So guess one; any will do as long as it's perpendicular to start
					// This implementation favors a rotation around the Up axis,
					// since it's often what you want to do.
					rotationAxis = Vec3::CrossProduct( Vec3( 0.0f, 0.0f, 1.0f ), start );
					if ( rotationAxis.Norm() < 0.01 ) // bad luck, they were parallel, try again!
						rotationAxis = Vec3::CrossProduct( Vec3( 1.0f, 0.0f, 0.0f ), start );

					rotationAxis.Normalize();
					return AngleAxis( 180.0f, rotationAxis );
				}

				// Implementation from Stan Melax's Game Programming Gems 1.0f article
				rotationAxis = Vec3::CrossProduct( start, dest );

				float s = sqrt( ( 1.0f + cosTheta ) * 2.0f );
				float invs = 1.0f / s;

				Quat quat;
				quat.w = s * 0.5f;
				quat.x = rotationAxis.x * invs;
				quat.y = rotationAxis.y * invs;
				quat.z = rotationAxis.z * invs;
				return quat;
			}

			Quat Quat::LookAt( Vec3 direction, Vec3 desiredUp )
			{
				if ( direction.Norm() < 0.0001f )
					return Quat();

				// Recompute desiredUp so that it's perpendicular to the direction
				// You can skip that part if you really want to force desiredUp
				Vec3 right = Vec3::CrossProduct( direction, desiredUp );
				desiredUp = Vec3::CrossProduct( right, direction );

				// Find the rotation between the front of the object (that we assume towards +Z,
				// but this depends on your model) and the desired direction
				Quat rot1 = RotationBetweenVectors( Vec3( 0.0f, 0.0f, 1.0f ), direction );
				// Because of the 1rst rotation, the up is probably completely screwed up. 
				// Find the rotation between the "up" of the rotated object, and the desired up
				Vec3 newUp = rot1.Rotate( Vec3( 0.0f, 1.0f, 0.0f ) );
				Quat rot2 = RotationBetweenVectors( newUp, desiredUp );

				// Apply them
				return rot2 * rot1; // remember, in reverse order.
			}

			Quat Quat::FromEulerAngle( Vec3 euler )
			{
				int intValX = static_cast<int>(euler.x);
				int intValY = static_cast<int>(euler.y);
				int intValZ = static_cast<int>(euler.z);

				if ( intValX == 180 && intValY == 180 )
				{
					euler.x = euler.y = 0.f;
					euler.z = -179.f;
				}
				else if ( intValY == 180 && intValZ == 180 )
				{
					euler.z = euler.y = 0.f;
					euler.x = -179.f;
				}

				if ( static_cast<int>(euler.x) == 180 )
					euler.x = 179.f;
				else if ( static_cast<int>(euler.x) == -180 )
					euler.x = -179.f;

				if ( static_cast<int>(euler.y) == 180 )
					euler.y = 179.f;
				else if ( static_cast<int>(euler.y) == -180 )
					euler.y = -179.f;

				if ( static_cast<int>(euler.z) == 180 )
					euler.z = 179.f;
				else if ( static_cast<int>(euler.z) == -180 )
					euler.z = -179.f;

				float heading = Utils::ToRadian( euler.y );
				float attitude = Utils::ToRadian( euler.x );
				float bank = Utils::ToRadian( euler.z );

				// Assuming the angles are in radians.
				float c1 = cos( heading );
				float s1 = sin( heading );
				float c2 = cos( attitude );
				float s2 = sin( attitude );
				float c3 = cos( bank );
				float s3 = sin( bank );
				Quat q;
				q.w = sqrt( 1.0f + c1 * c2 + c1 * c3 - s1 * s2 * s3 + c2 * c3 ) / 2.0f;
				float w4 = ( 4.0f * q.w );
				q.x = ( c2 * s3 + c1 * s3 + s1 * s2 * c3 ) / w4;
				q.y = ( s1 * c2 + s1 * c3 + c1 * s2 * s3 ) / w4;
				q.z = ( -s1 * s3 + c1 * s2 * c3 + s2 ) / w4;

				return q;
			}
		}
	}
}
