#pragma once

#include "TCMSceneObject.h"
#include "TCMComponent.h"
#include "TCMRenderer/Camera/CameraData.h"

namespace TCM
{
	namespace Engine
	{
		// * Base class for any Camera
		/**
		 * \brief \n Abstract class for every camera component. \n
		 */
		class ENGINECLASSES_DLL_EXPORT TCMCameraComponent : public TCMComponent
		{
			TCM_CLASS(TCMCameraComponent, TCMComponent);

			//TCM_SERIALIZE( TCM_SERIALIZED_VAR( cameraData ) );

		public:
			TCMCameraComponent();
			virtual ~TCMCameraComponent();

			/**
			 * \brief Pseudo-constructor used by pool
			 */
			void TCMPoolCreate() override;

			/**
			* \brief Pseudo-destructor used by pool
			*/
			void TCMPoolRelease() override;

			/**
			 * \brief Initialization method called after creation
			 */
			void Init() override;

			/**
			* \brief TCM function to manage updates when being modified editor
			* \param deltaTime _IN_ Deltatime since last call
			* \warning Should not be manually called nor overriden
			*/
			void TCMEditorUpdate( const float deltaTime ) override;

			bool TCMUpdate( const float deltaTime ) override;

			/**
			 * \brief Get the CameraData attached to this TCMCameraComponent
			 * \return The CameraData attached to this TCMCameraComponent.
			 */
			Graphics::Camera::CameraData* GetCameraData() const;

			/**
			 * \brief Check if camera is focused by mouse
			 */
			void CheckCameraFocus();

			/**
			 * \brief Connect InputManager to this camera
			 */
			void ConnectInputManager();

			/**
			* \brief Disconnect InputManager to this camera
			*/
			void DisconnectInputManager();

		private:

			/**
			* \brief Called when position is changed
			* \param position : new position of the scene object
			*/
			void OnPositionChanged( const Math::Vec3& position ) override;

			/**
			* \brief Called when rotation is changed
			* \param rotation : new rotation of the scene object
			*/
			void OnRotationChanged( const Math::Vec3& rotation ) override;

			/**
			* \brief Callback function
			*/
			void OnEnable() override;

			/**
			* \brief Callback function
			*/
			void OnDisable() override;

			void MoveForward() const;
			void MoveBackward() const;
			void MoveLeft() const;
			void MoveRight() const;
			void MoveUp() const;
			void MoveDown() const;
			void RotateRight() const;
			void RotateLeft() const;
			void Rotate() const;

			void PickObject() const;

			bool m_IsFocused = false;

			bool m_inputManagerConnected = false;

			Graphics::Camera::CameraData* cameraData;
		};
	}
}
