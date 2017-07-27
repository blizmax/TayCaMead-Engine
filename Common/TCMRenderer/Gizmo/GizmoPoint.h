#pragma once
#include "MathLib/ColorRGBA.h"
#include "MathLib/Vec3.h"

namespace TCM
{
	namespace Debug
	{
		struct GizmoPoint
		{
			GizmoPoint( Engine::Math::Vec3 p, Engine::Math::ColorRGBA color );
			Engine::Math::Vec3 m_p;
			Engine::Math::ColorRGBA m_color;
		};
	}
}
