#include "Vec3Helper.h"

namespace TCM
{
	namespace Editor
	{
		void Vec3Helper::SetX( SerializedInterface* var, const float x )
		{
			Engine::Math::Vec3 vec = Get( var );
			vec.x = x;
			Set( var, vec );
		}

		void Vec3Helper::SetY( SerializedInterface* var, const float y )
		{
			Engine::Math::Vec3 vec = Get( var );
			vec.y = y;
			Set( var, vec );
		}

		void Vec3Helper::SetZ( SerializedInterface* var, const float z )
		{
			Engine::Math::Vec3 vec = Get( var );
			vec.z = z;
			Set( var, vec );
		}
	}
}
