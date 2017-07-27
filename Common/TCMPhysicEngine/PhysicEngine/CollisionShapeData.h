#pragma once
#include "TCMPhysicEngine/PhysicEngineDLL.h"
#include "CollisionShapeType.h"
#include "MathLib/Vec3.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			/**
			* \brief \n Structure containing various informations about a Collision Shape. \n
			* Needed to create a Collision Shape. \n
			*/
			typedef struct TCMPHYSICENGINE_DLL CollisionShapeData
			{
				/**
				* \brief \n Constructor for Collision Shape Info. \n
				* \param shape _IN_	The shape of the Collision Shape.
				* \param offset _IN_	The offset of the Collision Shape.
				* \param scale _IN_	The scale of the Collision Shape. \n
				* \remarks A Collision Shape's shape is not supposed to change once created.
				*/
				CollisionShapeData( CollisionShapesType shape = NONE
				                    , Math::Vec3 offset = Math::Vec3()
				                    , Math::Vec3 scale = Math::Vec3() )
					: m_shape( shape )
					, m_offset( offset )
					, m_scale( scale )
				{
				}

				/**
				* \brief \n Shape of the Collision Shape.
				*/
				CollisionShapesType m_shape = NONE;

				/**
				* \brief \n Offset of the Collision Shape.
				*/
				Math::Vec3 m_offset = Math::Vec3();

				/**
				* \brief \n Scale of the Collision Shape.
				*/
				Math::Vec3 m_scale = Math::Vec3();

				/**
				* \brief \n Indicate to the Physic Engine the scale of the Collision Shape has changed.  \n
				* \remarks DO NOT TOUCH, VARIABLE USED INTERNALLY. \n
				* RESCALING CAN COST A LOT FOR THE ENGINE.
				*/
				bool m_scaleChanged = false;

				/**
				* \brief \n Resets the data structure to the initial values. \n
				*/
				void Reset()
				{
					m_shape = NONE;
					m_offset = Math::Vec3();
					m_scale = Math::Vec3();
					m_scaleChanged = false;
				}
			} CollisionShapeData;
		}
	}
}
