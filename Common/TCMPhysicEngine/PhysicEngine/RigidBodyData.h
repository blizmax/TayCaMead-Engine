#pragma once

#include "TCMPhysicEngine/PhysicEngineDLL.h"
#include "TCMPhysicEngine/PhysicEngine/AxisLiberties.h"
#include "MathLib/Vec3.h"
#include "MathLib/Quat.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			/**
			* \brief  \n Rigid Body data structure, containing everything a rigid body needs. \n
			*/
			typedef struct TCMPHYSICENGINE_DLL RigidBodyData
			{
				/**
				* \brief  \n Constructor for the Rigid Body Info. \n
				* \param mass _IN_	The mass of the rigid body.
				* \param inertia _IN_	The inertia of the rigid body.
				* \param friction _IN_	The friction factor of the rigid body.
				* \param restitution _IN_	The restitution factor of the rigid body.
				* \param rotation _IN_	The inital rotation of the rigid body.
				* \param eulerAngles _IN_	The initial euler angle of the rigid body.
				* \param useGravity _IN_	Will the rigid body use gravity ?
				* \param kinematic	_IN_	Will the rigid body be kinematic ?
				* \param frozenLiberties _IN_	The liberties of the rigid body.
				* \param name _IN_	The name of the rigid body. \n
				* \remarks Use either Euler Angles or Quaternion rotation, since they both override each others.
				*/
				RigidBodyData( const float& mass = 1.f
				               , const Math::Vec3& inertia = Math::Vec3()
				               , const float& friction = 1.f
				               , const float& restitution = 1.f
				               , const Math::Quat& rotation = Math::Quat()
				               , const Math::Vec3& eulerAngles = Math::Vec3()
				               , const bool& useGravity = true
				               , const bool& kinematic = false
				               , AxisLiberties frozenLiberties = AxisLiberties()
				               , const std::string& name = "RigidBody" )
					: m_mass( mass )
					, m_inertia( inertia )
					, m_friction( friction )
					, m_restitution( restitution )
					, m_rotation( rotation )
					, m_eulerAngles( eulerAngles )
					, m_useGravity( useGravity )
					, m_isKinematic( kinematic )
					, m_frozenLiberties( frozenLiberties )
				{
				}

				/**
				* \brief  \n The mass of the rigid body. \n
				*/
				float m_mass = 1.f;

				/**
				* \brief  \n The inertia of the rigid body. \n
				*/
				Math::Vec3 m_inertia = Math::Vec3();

				/**
				* \brief  \n The friction factor of the rigid body. \n
				*/
				float m_friction = 1.f;

				/**
				* \brief  \n The restitution factor of the rigid body \n
				*/
				float m_restitution = 1.f;

				/**
				* \brief  \n The initial rotation of the rigid body. \n
				*/
				Math::Quat m_rotation = Math::Quat();

				/**
				* \brief  \n The initial euler angles of the rigid body. \n
				*/
				Math::Vec3 m_eulerAngles = Math::Vec3();

				/**
				* \brief  \n Is the rigid body uses the gravity ? \n
				*/
				bool m_useGravity = true;

				/**
				* \brief  \n Is the rigid body a kinematic body ? \n
				*/
				bool m_isKinematic = false;

				/**
				* \brief  \n The axis liberties of the rigid body. \n
				*/
				AxisLiberties m_frozenLiberties = AxisLiberties();

				/**
				* \brief  \n Tell the Physic Engine to do every operation about the mass of the rigid body.\n
				* \remarks INTERNALLY USED VALUE, DON'T USE.
				*/
				bool m_massChanged = false;

				/**
				* \brief  \n Tell the Physic Engine to do every operation about the inertia of the rigid body. \n
				* \remarks INTERNALLY USED VALUE, DON'T USE.
				*/
				bool m_inertiaChanged = false;

				/**
				* \brief  \n Tell the Physic Engine to do every operation about the axis liberties of the rigid body. \n
				* \remarks INTERNALLY USED VALUE, DON'T USE.
				*/
				bool m_frozenLibertiesChanged = false;

				/**
				* \brief  \n Tell the Physic Engine to do every operation about the kinematic state of the rigid body. \n
				* \remarks INTERNALLY USED VALUE, DON'T USE.
				*/
				bool m_isKinematicChanged = false;

				/**
				* \brief  \n Resets the data structure to initial values. \n
				*/
				void Reset()
				{
					m_mass = 1.f;
					m_friction = 1.f;
					m_restitution = 1.f;
					m_inertia = Math::Vec3();
					m_rotation = Math::Quat();
					m_eulerAngles = Math::Vec3();
					m_useGravity = true;
					m_isKinematic = false;
					m_frozenLiberties = AxisLiberties();

					m_massChanged =
							m_inertiaChanged =
							m_frozenLibertiesChanged =
							m_isKinematicChanged = false;
				}
			} RigidBodyData;
		}
	}
}
