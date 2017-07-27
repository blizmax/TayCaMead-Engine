#pragma once
#include "GizmoPoint.h"

namespace TCM
{
	namespace Debug
	{
		struct GizmoLine
		{
			GizmoLine( Engine::Math::Vec3 p1, Engine::Math::Vec3 p2, Engine::Math::ColorRGBA color );
			GizmoPoint m_p1;
			GizmoPoint m_p2;
		};
	}
}
