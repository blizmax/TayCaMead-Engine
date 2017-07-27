#include "Vec4Helper.h"

namespace TCM
{
	namespace Editor
	{
		void Vec4Helper::SetX( SerializedInterface* var, const float x )
		{
			Engine::Math::Vec4 vec = Get( var );
			vec.x = x;
			Set( var, vec );
		}

		void Vec4Helper::SetY( SerializedInterface* var, const float y )
		{
			Engine::Math::Vec4 vec = Get( var );
			vec.y = y;
			Set( var, vec );
		}

		void Vec4Helper::SetZ( SerializedInterface* var, const float z )
		{
			Engine::Math::Vec4 vec = Get( var );
			vec.z = z;
			Set( var, vec );
		}

		void Vec4Helper::SetW( SerializedInterface* var, const float w )
		{
			Engine::Math::Vec4 vec = Get( var );
			vec.w = w;
			Set( var, vec );
		}
	}
}
