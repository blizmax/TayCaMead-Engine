#include "Vec2Helper.h"

namespace TCM
{
	namespace Editor
	{
		void Vec2Helper::SetX( SerializedInterface* var, const float x )
		{
			Engine::Math::Vec2 vec = Get( var );
			vec.x = x;
			Set( var, vec );
		}

		void Vec2Helper::SetY( SerializedInterface* var, const float y )
		{
			Engine::Math::Vec2 vec = Get( var );
			vec.y = y;
			Set( var, vec );
		}
	}
}
