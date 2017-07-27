#pragma once
#include "TCMPhysicEngine/PhysicEngineDLL.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			/**
			* \brief  \n Data Structure rulling the liberties the rigid body has. \n
			* \a Example: A rigid body with a \e posX member at false will not be able to move on the X axis.
			*/
			struct TCMPHYSICENGINE_DLL AxisLiberties
			{
				/**
				* \brief  \n Default Constructor. \n
				*/
				AxisLiberties() = default;

				/**
				* \brief  \n Is the object allowed to move on the X axis ? \n
				*/
				bool m_posX = true;

				/**
				* \brief  \n Is the object allowed to move on the Y axis ? \n
				*/
				bool m_posY = true;

				/**
				* \brief  \n Is the object allowed to move on the Z axis ? \n
				*/
				bool m_posZ = true;

				/**
				* \brief  \n Is the object allowed to rotate on the X axis ? \n
				*/
				bool m_rotX = true;

				/**
				* \brief  \n Is the object allowed to rotate on the Y axis ? \n
				*/
				bool m_rotY = true;

				/**
				* \brief  \n Is the object allowed to rotate on the Z axis ? \n
				*/
				bool m_rotZ = true;

				/**
				* \brief  \n Resets \b every liberties to true. \n
				*/
				void Reset()
				{
					m_posX = true;
					m_posY = true;
					m_posZ = true;

					m_rotX = true;
					m_rotY = true;
					m_rotZ = true;
				}
			};
		}
	}
}
