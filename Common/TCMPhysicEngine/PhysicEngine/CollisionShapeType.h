#pragma once
#include "TCMPhysicEngine/PhysicEngineDLL.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			/**
			* \brief \n Shapes for Collision Shapes.
			*/
			typedef enum TCMPHYSICENGINE_DLL ECollisionShapesType
			{
				NONE = 0,
				SPHERE,
				PLANE,
				TRIANGLE,
				CAPSULE,
				BOX,
				CYLINDER,
			} CollisionShapesType;
		}
	}
}
