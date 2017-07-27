#include "MathLib/MathUtils.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			namespace Utils
			{
				float ToRadian( const float& angle )
				{
					return static_cast<float>(PI * angle / 180.0f);
				}

				float ToDegree( const float& angle )
				{
					return angle != 0.f ? static_cast<float>(180.0f / ( PI * angle )) : 0.f;
				}

				Vec3 ToRadian( const Vec3& vec )
				{
					Vec3 v;
					v.x = ToRadian( vec.x );
					v.y = ToRadian( vec.y );
					v.z = ToRadian( vec.z );
					return v;
				}

				Vec3 ToDegree( const Vec3& vec )
				{
					Vec3 v;
					v.x = ToDegree( vec.x );
					v.y = ToDegree( vec.y );
					v.z = ToDegree( vec.z );
					return v;
				}

				Quat ToRadian( const Quat& angle )
				{
					Quat q;
					q.x = ToRadian( angle.x );
					q.y = ToRadian( angle.y );
					q.z = ToRadian( angle.z );
					q.w = ToRadian( angle.w );
					return q;
				}

				Quat ToDegree( const Quat& angle )
				{
					Quat q;
					q.x = ToDegree( angle.x );
					q.y = ToDegree( angle.y );
					q.z = ToDegree( angle.z );
					q.w = ToDegree( angle.w );
					return q;
				}
			}
		}
	}
}
