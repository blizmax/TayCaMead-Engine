#pragma once
#include "MathLib/Vec3.h"
#include "VarHelper.h"

namespace TCM
{
	namespace Editor
	{
		class Vec3Helper abstract : public VarHelper<Engine::Math::Vec3>
		{
		public:

			static void SetX( SerializedInterface* var, const float x );
			static void SetY( SerializedInterface* var, const float y );
			static void SetZ( SerializedInterface* var, const float z );
		};
	}
}
