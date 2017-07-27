#pragma once

#include "TCMBase.h"

#include "TCMRenderer/Rendering/RenderableObject.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief Base class for every ticking entity
		 */
		class ENGINECLASSES_DLL_EXPORT TCMTicking abstract : public TCMBase, public Graphics::RenderableObject
		{
			TCM_CLASS(TCMBase);

			TCM_SERIALIZE( m_bIsTicking );

		public:
			TCMTicking();
			virtual ~TCMTicking();

			/**
			 * \brief Initialization method called after creation
			 */
			virtual void Init();

			/**
			 * \brief TCM function to manage updates when being modified editor
			 * \param deltaTime _IN_ Deltatime since last call
			 * \warning Should not be manually called nor overriden
			 */
			virtual void TCMEditorUpdate( const float deltaTime );

			/**
			 * \brief TCM function to manage updates
			 * \param deltaTime _IN_ Deltatime since last call
			 * \return True if can update, false otherwise
			 * \warning Should not be manually called nor overriden
			 */
			virtual bool TCMUpdate( const float deltaTime );

			/**
			* \brief TCM function to manage fixed updates (physics)
			* \param fixedDeltaTime _IN_ fixed Deltatime since last call
			* \return True if can update, false otherwise
			* \warning Should not be manually called nor overriden
			*/
			virtual bool TCMFixedUpdate( const float fixedDeltaTime );

			/**
			 * \brief TCM function to manage rendering
			 * \return True if can render, false otherwise
			 * \warning Should not be manually called nor overriden
			 */
			virtual bool TCMRender() override;

			/**
			 * \brief TCM function to manage destroying
			 */
			void TCMDestroy();

			/**
			 * \brief Destroy this object
			 */
			void Destroy();

			/**
			 * \brief Get if need to be destroyed or not
			 * \return True is need to be destroyed, false otherwise
			 */
			bool HasToBeDestroyed() const;

			/**
			* \brief Get if need to be initialized or not
			* \return True is need to be initialized, false otherwise
			*/
			bool HasToBeInitialized() const;

		protected:
			/**
			 * \brief Update between two frames
			 * \param deltaTime _IN_ Delta time since last frame
			 */
			virtual void Update( const float deltaTime );

			/**
			* \brief Fixed update (physics)
			* \param fixedDeltaTime _IN_ Delta time since last frame
			*/
			virtual void FixedUpdate( const float fixedDeltaTime );

			/**
			 * \brief Renders an object
			 */
			virtual void RenderGizmos() override;

			/**
			 * \brief Called when destroying the instance
			 */
			virtual void OnDestroy() = 0;

			/**
			 * \brief Has this object called Init() yet
			 */
			bool m_bInit = false;

			/**
			 * \brief If the instance is actually ticking or not
			 * \note Default: true
			 */
			bool m_bIsTicking = true;

			/**
			 * \brief If the object needs to be destroyed
			 */
			bool m_bNeedDestroy = false;
		};
	}
}
