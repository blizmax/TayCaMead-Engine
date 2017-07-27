#pragma once

#include "MathLib/Vec2.h"
#include "VarHelper.h"

namespace TCM
{
	namespace Editor
	{
		class Vec2Helper abstract : public VarHelper<Engine::Math::Vec2>
		{
		public:
			static void SetX( SerializedInterface* var, const float x );
			static void SetY( SerializedInterface* var, const float y );
		};
	}
}
