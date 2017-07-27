#pragma once

#include "TCMPhysicEngine/PhysicEngineDLL.h"
#include "TCMPhysicEngine/PhysicEngine/RigidBody.h"
#include "TCMPhysicEngine/PhysicEngine/RigidBodyData.h"

#include "MathLib/Vec3.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			/**
			 * \brief \n The Physic Engine. \n
			 * Rules the physics in the game engine, and in the game. \n
			 * \remarks This class is an \b abstract \b basis for a physics engine. \n
			 * It's needing an implementation. \n
			 */
			class PhysicEngine abstract
			{
			public:
				/**
				 * \brief \n Default Destructor. \n
				 */
				virtual ~PhysicEngine() = default;

				/**
				* \brief \n Inits the Physic Engine with the gravity. \n
				*/
				static TCMPHYSICENGINE_DLL void Initialize( const Math::Vec3& gravity );

				/**
				 * \brief \n Runs the Physic Engine simulation \b for \b one \b step. \n
				 */
				static TCMPHYSICENGINE_DLL void Run( const float fixedDeltaTime );

				/**
				 * \brief \n Shutdowns the engine. \n
				 */
				static TCMPHYSICENGINE_DLL void Shutdown();

				/**
				 * \brief \n Sets the gravity used to calculate physics. \n
				 * \param gravity _IN_	The new gravity value.
				 */
				static TCMPHYSICENGINE_DLL void SetGravity( const Math::Vec3& gravity );

				/**
				 * \brief \n Gets the current gravity of the engine. \n
				 * \return The gravity.
				 */
				static TCMPHYSICENGINE_DLL const Math::Vec3& GetGravity();

				/**
				 * \brief \n Creates a Collision Shape containing all given data. \n
				 * \param shapeInfo _IN_	A pointer to the \c CollisionShapeData used to create a Collision Shape. \n
				 * \return A pointer to a \c CollisionShape created with the given \c CollisionShapeData pointer. \n
				 * \remarks Use this function to create a \c CollisionShape instead of its constructor !
				 */
				static TCMPHYSICENGINE_DLL CollisionShape* CreateCollisionShape( CollisionShapeData* shapeInfo );

				/**
				 * \brief \n Deletes the \c CollisionShape. \n
				 * \param shape The \c CollisionShape to delete.
				 */
				static TCMPHYSICENGINE_DLL void DeleteCollisionShape( CollisionShape* shape );

				/**
				 * \brief \n Creates a rigid body ruled by the laws of physics. \n
				 * \param rigidBodyInfo _IN_	A pointer to the data requiered to create a rigid body.
				 * \param shapeInfo _IN_	A pointer to the \c CollisionShapeData required to create a rigid body. \n
				 * \return A pointer to the newly created rigid body. \n
				 * \remarks Use this function to create a rigid body, not the constructor of \c RigidBody !
				 */
				static TCMPHYSICENGINE_DLL RigidBody* CreateRigidBody( RigidBodyData* rigidBodyInfo, CollisionShapeData* shapeInfo );

				/**
				 * \brief \n Deletes the rigid body. \n
				 * \param rigidBody _IN_	A pointer to the \c RigidBody to delete.
				 */
				static TCMPHYSICENGINE_DLL void DeleteRigidBody( RigidBody* rigidBody );

				static TCMPHYSICENGINE_DLL void DeactivateRigidBody( RigidBody* rigidBody );
				static TCMPHYSICENGINE_DLL void ActivateRigidBody( RigidBody* rigidBody );

				static TCMPHYSICENGINE_DLL const float& GetUnitConvertor();

				static TCMPHYSICENGINE_DLL Math::Vec3 GetDefaultInertie();
				static TCMPHYSICENGINE_DLL float GetDefaultFriction();
				static TCMPHYSICENGINE_DLL float GetDefaultRestitution();

				static TCMPHYSICENGINE_DLL void SetDefaultInertie( const Math::Vec3& vec );
				static TCMPHYSICENGINE_DLL void SetDefaultFriction( const float value );
				static TCMPHYSICENGINE_DLL void SetDefaultRestitution( const float value );

			protected:
				/**
				 * \brief Default Constructor.
				 */
				PhysicEngine();

				/**
				 * \brief \n Initialisation function to be overriden. \n
				 * \param gravity _IN_	the gravity value.
				 */
				virtual void CoreInitialize( const Math::Vec3& gravity ) = 0;

				/**
				 * \brief \n Run function to be overriden. \n
				 */
				virtual void CoreRun( const float fixedDeltaTime ) = 0;

				/**
				* \brief \n Shutdown function to be overriden. \n
				*/
				void BaseShutdown();

				/**
				 * \brief \n Shutdown function to be overriden. \n
				 */
				virtual void CoreShutdown() = 0;

				/**
				 * \brief \n \c CollisionShape creator function to be overriden. \n
				 * \param shapeInfo A pointer to the data for the \c CollisionShape. \n
				 * \return A pointer to the new \c CollisionShape.
				 */
				virtual CollisionShape* CoreCreateCollisionShape( CollisionShapeData* shapeInfo ) = 0;

				/**
				* \brief \n Rigid Body creator function to be overriden. \n
				* \param rigidBodyInfo _IN_	 A pointer to the \c RigidBodyData needed for the \c RigidBody.
				* \param shapeInfo _IN_	 A pointer to the \c CollisionShapeData needed for the \c RigidBody. \n
				* \return A pointer to the newly created \c RigidBody.
				*/
				RigidBody* BaseCreateRigidBody( RigidBodyData* rigidBodyInfo, CollisionShapeData* shapeInfo );

				/**
				 * \brief \n Rigid Body creator function to be overriden. \n
				 * \param rigidBodyInfo _IN_	 A pointer to the \c RigidBodyData needed for the \c RigidBody.
				 * \param shapeInfo _IN_	 A pointer to the \c CollisionShapeData needed for the \c RigidBody. \n
				 * \return A pointer to the newly created \c RigidBody.
				 */
				virtual RigidBody* CoreCreateRigidBody( RigidBodyData* rigidBodyInfo, CollisionShapeData* shapeInfo ) = 0;

				/**
				 * \brief \n Collision Shape destructor function to be overriden. \n
				 * \param shape _IN_	 A pointer to the \c CollisionShape to be destroyed.
				 */
				virtual void CoreDeleteCollisionShape( CollisionShape* shape ) = 0;

				/**
				* \brief \n Rigid Body creator function to be overriden. \n
				* \param rigidBody _IN_	 A pointer to the \c RigidBody to be destroyed.
				*/
				void BaseDeleteRigidBody( RigidBody* rigidBody );

				/**
				 * \brief \n Rigid Body creator function to be overriden. \n
				 * \param rigidBody _IN_	 A pointer to the \c RigidBody to be destroyed.
				 */
				virtual void CoreDeleteRigidBody( RigidBody* rigidBody ) = 0;

				/**
				 * \brief \n Gravity setter function to be overriden. \n
				 * \param gravity _IN_	The new gravity.
				 */
				virtual void CoreSetGravity( const Math::Vec3& gravity ) = 0;

				/**
				 * \brief \n Gravity getter function to be overriden. \n
				 * \return The gravity value.
				 */
				virtual const Math::Vec3& CoreGetGravity() { return m_gravity; };

				void BaseDeactivateRigidBody( RigidBody* rigidBody );

				virtual void CoreDeactivateRigidBody( RigidBody* rigidBody ) = 0;

				void BaseActivateRigidBody( RigidBody* rigidBody );

				virtual void CoreActivateRigidBody( RigidBody* rigidBody ) = 0;

				/**
				 * \brief \n The gravity used in the engine. \n
				 */
				Math::Vec3 m_gravity = { 0.0f, -9.8f, 0.0f };

				/**
				 * \brief \n The default intertie used in the engine. \n
				 */
				Math::Vec3 m_inertie = { 0.0f, -1.0f, 0.0f };

				/**
				 * \brief \n The default friction used in the engine. \n
				 */
				float m_friction = 1.0f;
				/**
				 * \brief \n The default resitution used in the engine. \n
				 */
				float m_restitution = 0.5f;

				/**
				 * \brief \n The pyshics layers. \n
				 */
				std::vector<std::string> m_layerVector; // TODO: Layer tab[][]

				std::vector<std::vector<bool>> m_layerMatrix;

				std::vector<RigidBody*>* m_rigidBodyList;

				float unitConvertor = 1.f;
			private:
				/**
				 * \brief \n Singleton of the Physic Engine \n
				 * Created Automaticaly. \n
				 * \return The single instance of the Physic Engine. \n
				 * \remarks Don't forget to define your TCMOverridedPhysicEngine in TCMPhysicEngine.cpp !
				 */
				static PhysicEngine& GetInstance();
			};
		}
	}
}
