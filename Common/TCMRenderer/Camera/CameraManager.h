#pragma once
#include "TCMRenderer/RendererDefines.h"
#include "TCMRenderer/PostProcess/PostProcess.h"
#include "CameraData.h"
#include <vector>

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			class RenderableObject;

			namespace Camera
			{
				class CameraManager final
				{
				public:
					/**
					 * \brief Render the RenderableObject with all cameras
					 * \param scene _IN_ Scene to render
					 */
					static TCMRENDERER_DLL void Render( RenderableObject* scene );
					/**
					 * \brief Get first main camera drawing
					 * \return Main camera
					 */
					static TCMRENDERER_DLL Camera::CameraData* GetMainCamera();

					/**
					 * \brief Get index of the first main camera
					 * \return
					 */
					static TCMRENDERER_DLL int GetMainCameraIndex();
					/**
					 * \brief Get one camera from all cameras
					 * \param index _IN_ Index of the camera
					 * \return Camera at index
					 */
					static TCMRENDERER_DLL Camera::CameraData* GetCamera( const int index );

					/**
					 * \brief Get index of the camera
					 * \param camera _IN_ Camera to find
					 * \return Index of the camera
					 */
					static TCMRENDERER_DLL int GetIndex( Camera::CameraData* camera );

					/**
					* \brief Get index of the camera
					* \param camera _IN_ Camera to find
					* \return Index of the camera
					*/
					static TCMRENDERER_DLL int GetIndex( Camera::CameraData const* camera );

					/**
					 * \brief Remove one camera from list
					 * \param index _IN_ Index of the camera
					 */
					static TCMRENDERER_DLL void UnregisterCamera( const int index );

					/**
					 * \brief Remove one camera from list
					 * \param camera _IN_ Camera to remove
					 */
					static TCMRENDERER_DLL void UnregisterCamera( Camera::CameraData* camera );

					/**
					* \brief Remove all cameras from list
					*/
					static TCMRENDERER_DLL void UnregisterAllCameras();

					/**
					 * \brief Get cameras count
					 * \return
					 */
					static TCMRENDERER_DLL size_t CamerasCount();

					/**
					 * \brief Register the camera, add it to the list
					 * \param camera _IN_ Camera to register
					 */
					static TCMRENDERER_DLL void RegisterCamera( Camera::CameraData* camera );

					/**
					 * \brief Set the camera count to render on screen
					 * \param count _IN_ Count of cameras
					 */
					static TCMRENDERER_DLL void SetCameraCountOnScreen( const int count );
					/**
					 * \brief Get camera count rendered on screen
					 * \return
					 */
					static TCMRENDERER_DLL size_t GetCameraCountOnScreen();

					/**
					 * \brief Switch to the next camera
					 */
					static TCMRENDERER_DLL void SwitchCameras();

					/**
					 * \brief Force sorting of cameras, depending of the depth
					 */
					static void Sort();

					static void ResizeViewport( const int width, const int height );

					static TCMRENDERER_DLL void SetMSAASettings( bool enabled, int count );

					static TCMRENDERER_DLL bool IsMSAAEnabled();

					static TCMRENDERER_DLL int GetMSAASamplesCount();

					static FrameBuffer* CreateRendererFramebuffer( const Math::Vec2& size );

				protected:
					static CameraManager& GetInstance();

				private:

					/**
					* \brief Render the RenderableObject with all cameras
					* \param scene _IN_ Scene to render
					*/
					void CoreRender( RenderableObject* scene );

					/**
					* \brief Get first main camera drawing
					* \return Main camera
					*/
					Camera::CameraData* CoreGetMainCamera();

					/**
					* \brief Get index of the first main camera
					* \return
					*/
					int CoreGetMainCameraIndex() const;

					/**
					* \brief Get one camera from all cameras
					* \param index _IN_ Index of the camera
					* \return Camera at index
					*/
					Camera::CameraData* CoreGetCamera( const int index );

					/**
					* \brief Get index of the camera
					* \param camera _IN_ Camera to find
					* \return Index of the camera
					*/
					int CoreGetIndex( Camera::CameraData const* camera );

					/**
					* \brief Get index of the camera
					* \param camera _IN_ Camera to find
					* \return Index of the camera
					*/
					int CoreGetIndex( Camera::CameraData* camera );

					/**
					* \brief Remove one camera from list
					* \param index _IN_ Index of the camera
					*/
					void CoreRemoveCamera( const int index );

					/**
					* \brief Remove one camera from list
					* \param camera _IN_ Camera to remove
					*/
					void CoreRemoveCamera( Camera::CameraData* camera );

					/**
					* \brief Remove all cameras from list
					*/
					void CoreUnregisterAllCameras();

					/**
					* \brief Get cameras count
					* \return
					*/
					size_t CoreCamerasCount() const;

					/**
					* \brief Register the camera, add it to the list
					* \param camera _IN_ Camera to register
					*/
					void CoreRegisterCamera( Camera::CameraData* camera );

					/**
					* \brief Set the camera count to render on screen
					* \param count _IN_ Count of cameras
					*/
					void CoreSetCameraCountOnScreen( const int count );

					/**
					* \brief Get camera count rendered on screen
					* \return
					*/
					size_t CoreGetCameraCountOnScreen() const;

					/**
					* \brief Switch to the next camera
					*/
					void CoreSwitchCameras();

					/**
					* \brief Force sorting of cameras, depending of the depth
					*/
					void CoreSort();

					void CoreSetMSAASettings( bool enabled, int count );

					void CoreResizeViewport( const int width, const int height );

					int m_countCamerasOnScreen = 1;
					int m_currentCamera = 0;
					std::vector<Camera::CameraData *> m_cameras;

					bool m_HDRenabled = false;
					bool m_MSAAEnabled = true;
					int m_MSAASamples = 2;
				};
			}
		}
	}
}
