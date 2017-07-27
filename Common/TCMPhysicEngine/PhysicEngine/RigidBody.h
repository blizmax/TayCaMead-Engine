#pragma once

#include "TCMPhysicEngine/PhysicEngineDLL.h"
#include "MathLib/Vec3.h"
#include "MathLib/Quat.h"
#include "RigidBodyData.h"
#include "CollisionShape.h"
#include "CollisionData.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			/**
			* \brief  \n A rigid body, allowing an object to be ruled by the laws of physics. \n
			* \remarks This structure is supposed to be \b overriden, it's providing only the basis for a rigid body.
			*/
			struct TCMPHYSICENGINE_DLL RigidBody
			{
				/**
				* \brief  \n Default Constructor. \n
				* \note Do yourself a favor and use : \n
				* ->	\c RigidBody( \p const \c RigidBodyData& \a info ) \n
				*/
				RigidBody() = default;

				/**
				* \brief  \n Constructor for using a \c RigidBodyData data structure. \n
				* \param infoR _IN_	The info for the rigid body.
				*/
				explicit RigidBody( RigidBodyData* infoR )
					: m_rigidBodyInfo( infoR )
					, m_collisionShape( nullptr )
				{
				}

				/**
				* \brief  \n Default Destructor. \n
				* \remarks Don't forget to overide when derivating the structure.
				*/
				virtual ~RigidBody();

				/**
				* \brief  \n Gets the Collision Shape containing all data for a Collision Shape. \n
				* \return The \c CollisionShape stated above.
				*/
				virtual CollisionShape* GetCollisionShape() const;

				/**
				* \brief  \n Gets the mass of the rigid body. \n
				* \return The mass of the rigid body.
				*/
				virtual float GetMass() const;

				/**
				* \brief  \n Sets the mass of the rigid body. \n
				* \param mass _IN_	 The new mass of the rigid body.
				*/
				virtual void SetMass( float mass );

				/**
				* \brief  \n Gets the inertia of the rigid body. \n
				* \return The inertia of the rigid body.
				*/
				virtual Math::Vec3 GetInertia() const;

				/**
				* \brief  \n Sets the inertia of the rigid body. \n
				* \param inertia _IN_	The inertia of the rigid body.
				*/
				virtual void SetInertia( Math::Vec3 inertia );

				/**
				* \brief  \n Is the rigid body using the gravity ? \n
				* \return The answer to the question stated above.
				*/
				virtual bool IsUsingGravity() const;

				/**
				* \brief  \n Will the rigid body be ruled by the laws of gravity ? \n
				* \param val _IN_		Answer to the question stated above.
				*/
				virtual void UseGravity( bool val );

				/**
				* \brief  \n Is the rigid body kinematic ? \n
				* \return The answer to the question stated above. \n
				* \note A kinematic body is a body free of laws of gravity. \n
				* Can collide with dynamic bodies, contrary to static bodies.
				*/
				virtual bool IsKinematic() const;

				/**
				* \brief  \n Will the rigid body be kinematic ? \n
				* \param val _IN_		The answer to the question stated above. \n
				* \note A kinematic body is a body free of laws of gravity. \n
				* Can collide with dynamic bodies, contrary to static bodies.
				*/
				virtual void IsKinematic( bool val );

				/**
				* \brief  \n Gets the axis liberties of the rigid body. \n
				* \return The Axis liberties of the rigid body.
				*/
				virtual AxisLiberties GetLiberties() const;

				/**
				* \brief  \n Sets the axis liberties of the rigid body. \n
				* \param liberties _IN_	 The new axis liberties of the rigid body.
				*/
				virtual void SetLiberties( const AxisLiberties& liberties );

				/**
				* \brief  \n Gets the position of the rigid body. \n
				* \return The position of the rigid body.
				*/
				virtual Math::Vec3 GetPosition() const = 0;

				/**
				* \brief  \n Sets the position of the rigid body. \n
				* \param position _IN_	The new position of the rigid body.
				*/
				virtual void SetPosition( Math::Vec3 position ) = 0;

				/**
				* \brief  \n Gets the rotation of the rigid body. \n
				* \return The rotation of the rigid body.
				*/
				virtual Math::Quat GetRotation() const;

				/**
				* \brief  \n Sets the rotation of the rigid body. \n
				* \param rotation _IN_	The new rotation of the rigid body.
				*/
				virtual void SetRotation( const Math::Quat& rotation );

				/**
				* \brief  \n Gets the euler angles of the rigid body. \n
				* \return The euler angles of the rigid body.
				*/
				virtual Math::Vec3 GetEulerAngles() const;

				/**
				* \brief  \n Sets the euler angles of the rigid body. \n
				* \param angles _IN_	The new euler angles of the rigid body.
				*/
				virtual void SetEulerAngles( const Math::Vec3& angles );

				/**
				* \brief  \n Gets the friction factor of the rigid body. \n
				* \return The friction factor of the rigid body.
				*/
				virtual float GetFriction() const;

				/**
				* \brief  \n Sets the friction factor of the rigid body. \n
				* \param friction _IN_	The new friction factor of the rigid body.
				*/
				virtual void SetFriction( const float& friction );

				/**
				* \brief  \n Gets the restitution factor of the rigid body. \n
				* \return The restitution factor of the rigid body.
				*/
				virtual float GetRestitution() const;

				/**
				* \brief  \n Sets the restitution factor of the rigid body. \n
				* \param restitution _IN_	The restitution factor of the rigid body.
				*/
				virtual void SetRestitution( const float& restitution );

				/**
				* \brief  \n Overridable function, called every frame of the engine.\n
				*/
				virtual void Update() = 0;

				/**
				* \brief  \n Gets the rigid body data structure of the rigid body. \n
				* \return The rigid body data structure of the rigid body.
				*/
				virtual RigidBodyData* GetRigidBodyInfo();

				/**
				* \brief  \n Sets the rigid body data of the rigid body. \n
				* \param bodyInfo _IN_	The new data structure of the rigid body.
				*/
				virtual void SetRigidBodyInfo( RigidBodyData* bodyInfo );

				CollisionData* m_collisionData = nullptr;

			protected:
				/**
				* \brief  \n The data structure of the rigid body. \n
				*/
				RigidBodyData* m_rigidBodyInfo;

				CollisionShape* m_collisionShape;
			};
		}
	}
}
