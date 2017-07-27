#pragma once
#include "MathLib/Vec4.h"
#include "VarHelper.h"

namespace TCM
{
	namespace Editor
	{
		class Vec4Helper abstract : public VarHelper<Engine::Math::Vec4>
		{
		public:
			static void SetX( SerializedInterface* var, const float x );
			static void SetY( SerializedInterface* var, const float y );
			static void SetZ( SerializedInterface* var, const float z );
			static void SetW( SerializedInterface* var, const float w );
		};
	}
}
