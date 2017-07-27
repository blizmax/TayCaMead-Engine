#pragma once

#include <memory>
#include <mutex>

#include "MathLib/Mat4.h"
#include "TCMTicking.h"
#include "TCMMemory/TCMPoolObject.h"
#include "TCMMemory/TCMDynamicPoolManager.hpp"

#include "TCMLogger/Logger.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			struct RigidBody;
		}

		class TCMSceneObject;

		/**
		 * \brief Base class for every component contained in a TCMSceneObject
		 */
		class ENGINECLASSES_DLL_EXPORT TCMComponent abstract : public TCMTicking, public Memory::TCMPoolObject
		{
			TCM_CLASS(TCMTicking);

		public:
			TCMComponent();
			virtual ~TCMComponent();

			/**
			 * \brief Pseudo-constructor used instanciated by pool
			 */
			void TCMPoolCreate() override;

			/**
			 * \brief Pseudo-destructor used when released by pool
			 */
			void TCMPoolRelease() override;

#ifdef _DEBUG
			void* operator new( size_t size )
			{
				return ::operator new( size );
			};

			void operator delete( void* ptr, size_t size )
			{
				return ::operator delete( ptr, size );
			};
#endif

			/**
			 * \brief Get World Matrix, already transposed
			 * \return World Matrix of the object containing the component
			 */
			Math::Mat4 GetWorldMatrix() const;

			/**
			 * \brief Set this TCMComponent's owner
			 * \param sceneObject _IN_ TCMSceneObject that will be the owner
			 */
			void SetSceneObject( TCMSceneObject* sceneObject );

			/**
			 * \brief Removes this TCMComponent from its TCMSceneObject
			 */
			void RemoveFromSceneObject();

			/**
			 * \brief Called when position is changed
			 * \param position : new position of the scene object
			 */
			virtual void OnPositionChanged( const Math::Vec3& position );

			/**
			* \brief Called when rotation is changed
			* \param rotation : new rotation of the scene object
			*/
			virtual void OnRotationChanged( const Math::Vec3& rotation );

			/**
			* \brief Called when scale is changed
			* \param scale : new scale of the scene object
			*/
			virtual void OnScaleChanged( const Math::Vec3& scale );

			/**
			 * \brief Called when world matrix is changed
			 * \param mat : new world matrix
			 */
			virtual void OnWorldMatrixChanged( const Math::Mat4& mat );

			/**
			* \brief TCM function to manage gizmos rendering
			* \warning Should not be manually called nor overriden
			*/
			void RenderGizmos() override final;

			virtual void OnDrawGizmos();

			virtual void OnDrawGizmosSelected();

			virtual void OnCollisionEnter( TCM::Engine::Physics::RigidBody* body );
			virtual void OnCollisionExit();

			/**
			 * \brief Owner of this component
			 */
			TCMSceneObject* m_sceneObject;

		protected:
			/**
			 * \brief Called when destroying the instance
			 */
			virtual void OnDestroy() override;

			bool SceneObjIsSelected() const;

#pragma region Static - Factory / Pool
		public:
			/**
			 * \brief Creates a TCMComponent
			 * \tparam TDerived The derived type of the TCMComponent to be instanciated
			 * \return A new object of type TDerived
			 * \note Don't always call type constructor. Calls TCMPoolCreate(), but no Construct()
			 */
			template<class TDerived = TCMComponent>
			static TDerived* Create()
			{
				TCMDEBUG("");
				return GetInstance().New<TDerived>();
			};

			/**
			 * \brief Creates a TCMComponent
			 * \tparam TDerived The derived type of the TCMComponent to be instanciated
			 * \tparam Args The types of the arguments taken by the Construct()
			 * \param args _IN_ The arguments sent to Construct()
			 * \return A new object of type TDerived
			 * \note Don't always call type constructor. Calls TCMPoolCreate(), and Construct(args)
			 */
			template<class TDerived = TCMComponent, typename... Args>
			static TDerived* Create( Args&&... args )
			{
				TCMDEBUG("");
				return GetInstance().New<TDerived>( args... );
			};

			/**
			 * \brief Releases object
			 * \param object _IN_ The TCMComponent to be deleted
			 * \note Will never call type destructor. Calls TCMPoolRelease() only
			 */
			static void Release( TCMComponent* object )
			{
				TCMDEBUG("");
				GetInstance().Delete( object );
			};

		private:
			/**
			 * \brief Get the unique instance of the TCMDynamicPoolManager<TCMComponent>
			 * \return A reference to the TCMDynamicPoolManager<TCMComponent>
			 */
			static Memory::TCMDynamicPoolManager<TCMComponent>& GetInstance()
			{
				static std::unique_ptr<Memory::TCMDynamicPoolManager<TCMComponent>> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new Memory::TCMDynamicPoolManager<TCMComponent>() );
				                }
				);
				return *( m_instance.get() );
			};
#pragma endregion
		};
	}
}
