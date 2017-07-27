#pragma once
#include "EngineClassesDefines.h"
#include "TCMComponent.h"
#include "TCMPhysicEngine/PhysicEngine/RigidBodyData.h"
#include "TCMPhysicEngine/PhysicEngine/CollisionShape.h"
#include "TCMPhysicEngine/PhysicEngine/AxisLiberties.h"
#include "MathLib/Quat.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct s_AbstractMesh;
		}

		/**
		 * \brief \n Component for rigid body, allowing collision to be done. \n
		 */
		class ENGINECLASSES_DLL_EXPORT TCMComponentRigidBody abstract : public TCMComponent
		{
			TCM_CLASS(TCMComponent);

			TCM_SERIALIZE( m_collisionShapeInfo, m_rigidBodyInfo );

		public:
			TCMComponentRigidBody();
			virtual ~TCMComponentRigidBody();

			void Init() override;
			void TCMPoolRelease() override;
			void TCMPoolCreate() override;

			void OnDrawGizmos() override;

			void Extend( const Math::Vec3 extend ) const;
			void Offset( const Math::Vec3 offset ) const;

			/**
			 * \brief \n Gets the collision shape of the rigid body. \n
			 * \return The collision shape of the rigid body.
			 */
			Physics::CollisionShape* GetCollisionShape() const;

			/**
			 * \brief \n Gets the mass of the rigid body. \n
			 * \return The mass of the rigid body.
			 */
			float GetMass() const;
			/**
			 * \brief \n Sets the mass of the rigid body. \n
			 * \param mass _IN_	The mass of the rigid body.
			 */
			void SetMass( float mass ) const;

			/**
			* \brief \n Gets the inertia of the rigid body. \n
			* \return The inertia of the rigid body.
			*/
			Math::Vec3 GetInertia() const;
			/**
			* \brief \n Sets the inertia of the rigid body. \n
			* \param inertia _IN_	The inertia of the rigid body.
			*/
			void SetInertia( const Math::Vec3& inertia ) const;

			/**
			 * \brief \n Returns if the rigid body is using gravity. \n 
			 * \return \p true if using gravity, \p false if not.
			 */
			bool IsUsingGravity() const;
			/**
			 * \brief \n Tells if the rigid body should use gravity or not. \n 
			 * \param val _IN_	The value for using gravity.
			 */
			void SetUseGravity( bool val ) const;

			/**
			 * \brief \n Returns if the rigid body is kinematic. \n 
			 * \return \p true if kinematic, \p false if not.
			 */
			bool IsKinematic() const;
			/**
			 * \brief \n Tells if the rigid body should be kinematic or not. \n 
			 * \param val _IN_	The value for being kinematic.
			 */
			void IsKinematic( bool val ) const;

			/**
			 * \brief \n Gets the axis liberties of the rigid body. \n
			 * \return The axis liberties of the rigid body.
			 */
			const Physics::AxisLiberties& GetLiberties() const;
			/**
			 * \brief \n Sets the axis liberties of the rigid body. \n
			 * \param liberties _IN_	The new axis liberties of the rigid body.
			 */
			void SetLiberties( const Physics::AxisLiberties& liberties ) const;

			/**
			 * \brief \n Gets the position of the rigid body. \n
			 * \return The position of the rigid body.
			 */
			Math::Vec3 GetPosition() const;
			/**
			 * \brief \n Sets the position of the rigid body. \n
			 * \param position _IN_	The new position of the rigid body.
			 */
			void SetPosition( const Math::Vec3& position ) const;

			/**
			 * \brief \n Gets the rotation of the rigid body. \n
			 * \return The rotation of the rigid body.
			 */
			const Math::Quat& GetRotation() const;
			/**
			 * \brief \n Sets the rotation of the rigid body. \n
			 * \param rotation _IN_	The new rotation of the rigid body.
			 */
			void SetRotation( const Math::Quat& rotation ) const;

			/**
			 * \brief \n Gets the euler angles of the rigid body. \n
			 * \return The euler angles of the rigid body.
			 */
			const Math::Vec3& GetEulerAngles() const;

			/**
			 * \brief \n Sets the euler angles of the rigid body. \n
			 * \param angles _IN_	The new euler angles of the rigid body.
			 */
			void SetEulerAngles( const Math::Vec3& angles ) const;

			/**
			 * \brief \n Gets the friction of the rigid body. \n
			 * \return The friction of the rigid body.
			 */
			float GetFriction() const;
			/**
			 * \brief \n Sets the friction of the rigid body. \n
			 * \param friction _IN_	The new friction of the rigid body.
			 */
			void SetFriction( const float& friction ) const;

			/**
			 * \brief \n Gets the restitution of the rigid body. \n
			 * \return The restitution of the rigid body.
			 */
			float GetRestitution() const;
			/**
			 * \brief \n Sets the restitution of the rigid body. \n
			 * \param restitution _IN_	The new restitution of the rigid body.
			 */
			void SetRestitution( const float& restitution ) const;

			void OnEnable() override;
			void OnDisable() override;

			void OnCollisionEnter( Physics::RigidBody* body ) override;
			void OnCollisionExit() override;

			void OnScaleChanged( const Math::Vec3& scale ) override;

		protected:
			void FixedUpdate( const float fixedDeltaTime ) override;
			void OnDestroy() override;

			void InitRigidbody();

			Physics::CollisionShapeData* m_collisionShapeInfo = nullptr;

			Graphics::s_AbstractMesh* m_mesh = nullptr;

			Physics::RigidBody* m_rigidBody = nullptr;

			Physics::RigidBodyData* m_rigidBodyInfo = nullptr;
		};
	}
}
