#pragma once

#include <vector>
#include "TCMTicking.h"

#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		class TCMCameraComponent;

		/**
		 * \brief base class for every Scene
		 */
		class ENGINECLASSES_DLL_EXPORT TCMScene final : public TCMTicking
		{
			TCM_CLASS(TCMScene, TCMTicking);

		public:
			TCMScene();
			virtual ~TCMScene();

			/**
			 * \brief TCM function to manage updates when being modified editor
			 * \param deltaTime _IN_ Deltatime since last call
			 * \warning Should not be manually called nor overriden
			 */
			virtual void TCMEditorUpdate( const float deltaTime ) override final;

			/**
			 * \brief Initialization method called after creation
			 */
			virtual void Init() override;

			/**
			 * \brief TCM function to manage updates
			 * \param deltaTime _IN_ Deltatime since last call
			 * \return True if can update, false otherwise
			 * \warning Should not be manually called nor overriden
			 */
			virtual bool TCMUpdate( const float deltaTime ) override final;

			/**
			* \brief TCM function to manage fixed updates (physics)
			* \param fixedDeltaTime _IN_ fixed Deltatime since last call
			* \return True if can update, false otherwise
			* \warning Should not be manually called nor overriden
			*/
			virtual bool TCMFixedUpdate( const float fixedDeltaTime ) override final;

			/**
			 * \brief TCM function to manage rendering
			 * \return True if can render, false otherwise
			 * \warning Should not be manually called nor overriden
			 */
			virtual bool TCMRender() override final;

			/**
			 * \brief Set if the instance should be enabled or not
			 * \param bEnabled _IN_ The new state of the instance
			 */
			virtual void SetEnabled( const bool bEnabled, const bool callback = true ) override;

			/**
			 * \brief Adds a TCMSceneObject to the TCMScene
			 * \param sceneObject _IN_ The TCMSceneObject to be added
			 */
			void AddSceneObject( TCMSceneObject* sceneObject ) const;

			/**
			 * \brief Removes a TCMSceneObject from the TCMScene
			 * \param sceneObject _IN_ The TCMSceneObject to be removed
			 * \return True if the object was removed, false otherwise
			 */
			bool RemoveSceneObject( TCMSceneObject* sceneObject ) const;

			/**
			 * \brief Sets the skyfaces
			 * \param skyfaces The paths to the skybox images
			 */
			void SetSkyfaces( std::string skyfaces[6] );

			/**
			 * \brief Get the skyfaces
			 * \return The paths to the skybox images
			 */
			const std::vector<std::string>* GetSkyfaces() const;

			/**
			 * \brief Gets all the TCMSceneObject's of one type
			 * \tparam T The type of the TCMSceneObject to get
			 * \return A vector of every typed TCMSceneObject of the TCMScene
			 */
			template<typename T>
			typename std::enable_if<std::is_base_of<TCMComponent, T>::value, std::vector<TCMSceneObject *>>::type GetAllSceneObjects() const
			{
				return m_rootObject->DeepGetAllChildren<T>();
			};

			/**
			* \brief Gets roots TCMSceneObject's of one type
			* \tparam T The type of the TCMSceneObject to get
			* \return A vector of every typed TCMSceneObject of the TCMScene
			*/
			template<typename T>
			typename std::enable_if<std::is_base_of<TCMComponent, T>::value, std::vector<TCMSceneObject *>>::type GetRootSceneObjects() const
			{
				return m_rootObject->GetAllChildren<T>();
			};

			/**
			 * \brief Gets all the TCMSceneObject's in the scene
			 * \return A vector of every TCMSceneObject of the TCMScene
			 */
			std::vector<TCMSceneObject *> GetAllSceneObjects() const;

			/**
			* \brief Gets roots TCMSceneObject's in the scene
			* \return A vector of every TCMSceneObject of the TCMScene
			*/
			std::vector<TCMSceneObject *> GetRootSceneObjects() const;

			/**
			* \brief Gets root TCMSceneObject's in the scene
			* \return the TCMSceneObject of the TCMScene
			*/
			TCMSceneObject* GetRootSceneObject() const;

			/**
			 * \brief Switch to ovni camera
			 */
			void SwitchOvniMode();

			/**
			 * \brief Switch to ovni camera
			 * \param value _in_ Ovni camera enabled
			 */
			void SwitchOvniMode( bool value );

			/**
			 * \brief Set Ovni camera direction to look at
			 * \param object _IN_ Object to look at
			 * \warning Don't work 
			 */
			void OvniLookAt( TCMSceneObject* object ) const;

		protected:
			/**
			 * \brief Called when destroying the instance
			 */
			virtual void OnDestroy() override;

			void RenderGizmos() override;

		private:

			/**
			 * \brief The current active Skybox
			 */
			Graphics::AbstractSkybox* m_skyboxData = nullptr;

			std::vector<std::string>* m_skyfaces = nullptr;

			TCMSceneObject* m_rootObject = nullptr;

			TCMSceneObject* m_ovni;

			bool m_bOvniMode;
		};
	}
}
