#pragma once
#include "TCMRenderer/RendererDefines.h"
#include "TCMRenderer/PostProcess/PostProcess.h"
#include "TCMRenderer/Camera/ViewProjectionMatrix.h"
#include "MathLib/Vec2.h"
#include "MathLib/Vec3.h"
#include "MathLib/Bounds.h"
#include "MathLib/ColorRGB.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			enum class PostProcessEffect;

			namespace Camera
			{
				struct TCMRENDERER_DLL CameraData final
				{
					CameraData( Math::Vec2 ratio = { 1.0f, 1.0f }, bool debugCamera = false );
					~CameraData();

					/**
					* \brief Init the Camera Data
					*/
					void Init();

					/**
					* \brief Clear framebuffer
					*/
					void Clear() const;

					/**
					* \brief Render the camera
					*/
					void Render();

					/**
					* \brief Resize the camera
					*/
					void Resize();

					/**
					* \brief Force the rendering if not enable
					*/
					void ForceRender();

					void ForceBufferCreation();

					/**
					* \brief Callback when camera component is moved
					* \param position _IN_ New world position
					*/
					void OnPositionChanged( const Math::Vec3& position );

					/**
					* \brief Callback when camera component is rotated
					* \param rotation _IN_ New world rotation
					*/
					void OnRotationChanged( const Math::Vec3& rotation );

					/**
					* \brief Set fov in degree for the camera
					* \param degree _IN_ Angle of the fov in degree
					*/
					void SetFov( const float& degree );

					/**
					* \brief Set the near distance of the camera
					* \param value _IN_ Distance of the near
					*/
					void SetNearDistance( const float& value );

					/**
					* \brief Set the far distance of the camera
					* \param value _IN_ Distance of the far
					*/
					void SetFarDistance( const float& value );

					/**
					* \brief Set the ortho size for ortho camera
					* \param value _IN_ Ortho size
					*/
					void SetOrthoSize( const float& value );

					/**
					* \brief Set the depth of the camera, used sort the cameras
					* \param value _IN_ Depth value
					*/
					void SetDepth( const int value );

					/**
					* \brief Enable orthograpic camera
					* \param value _IN_ Enable camera
					*/
					void SetOrthographic( const bool value );

					void SetEnable( const bool value );

					void SetRatio( const Math::Vec2& ratio );

					float GetFovDegree() const;
					float GetFovRadian() const;
					float GetNearDistance() const;
					float GetFarDistance() const;
					float GetOrthoSize() const;

					int GetDepth() const;
					/**
					* \brief Get ratio of the camera depending of the screen width and height
					* \return Ratio of camera
					*/
					Math::Vec2 GetRatio() const;
					/**
					* \brief Get position of the camera depending of the screen width and height
					* \return Position of camera
					*/
					Math::Vec3 GetPosition() const;
					/**
					* \brief Get Camera Framebuffer used to render
					* \return Framebuffer
					*/
					FrameBuffer* GetFramebuffer() const;
					/**
					* \brief Get the created texture from rendering
					* \return Texture of rendering
					*/
					Graphics::AbstractTexture* GetTexture() const;

					Graphics::CameraSpecificData* GetSpecificData() const;

					/**
					* \brief Check if canera is drawn on screen
					* \return
					*/
					bool IsOnScreen() const;

					/**
					* \brief Check is camera is focused
					* \param mousePos _IN_ Mouse position in viewport
					* \return Camera focused or not
					*/
					bool IsFocused( const Math::Vec2& mousePos ) const;

					bool IsSphereSeen( const Math::Bounds::Sphere& sphere ) const;

					bool IsOrhtographic() const;

					/**
					* \brief Add PostProcessEffect at the end of the stack
					* \param effect _IN_ Type of the post process
					*/
					void AddPostProcessEffect( const Graphics::PostProcessEffect& effect ) const;

					/**
					* \brief Add PostProcessEffect at the beginning of the list
					* \param effect _IN_ Type of the post process
					*/
					void AddFirstPostProcessEffect( const Graphics::PostProcessEffect& effect ) const;

					/**
					* \brief Remove first PostProcessEffect of the list
					* \param effect _IN_ Type of the post process
					*/
					void RemovePostProcessEffect( const Graphics::PostProcessEffect& effect ) const;
					/**
					* \brief Remove first post process of the list
					*/
					void RemoveFirstPostProcessEffect() const;
					/**
					* \brief Remove last post process of the list
					*/
					void RemoveLastPostProcessEffect() const;
					/**
					* \brief Set list of post processes
					* \param effects _IN_ New list of post process
					*/
					void SetPostProcessEffects( const std::vector<Graphics::PostProcessEffect>& effects ) const;
					/**
					* \brief Clear post process list
					*/
					void ClearPostProccesses() const;

					/**
					* \brief Returns a ray going from camera through a viewport point.
					* \param position _IN_ Viewport coordinates normalized and relative to the camera.
					* The bottom-left of the camera is (0,0); the top-right is (1,1).
					*/
					const Graphics::RenderData* ViewportPointToRay( const Math::Vec2& position ) const;

					/**
					* \brief Return post process list of the camera
					* \return Pointer to the list
					*/
					std::vector<Graphics::PostProcess::PostProcess *>* GetPostProcessEffects() const;

					bool m_enabled = true;

					bool m_debugCamera;

					/**
					* \brief Will be overrided each frame
					*/
					bool m_isOnScreen = false;

					// * Position in viewport
					Math::Vec2 m_position = { 0.0f, 0.0f };

					Math::ColorRGB m_color = { 0.0f, 0.0f, 0.0f };

					ViewProjectionMatrix* m_viewProjectionMatrix;

				private:

					void UpdateCameraTransform();
					void UpdateCameraParams();
					void UpdateMVP();
					void UpdateFrustum( const Math::Mat4 clip );

					float m_fov = 45.f;
					float m_nearDistance = 0.1f;
					float m_farDistance = 1000.0f;

					float m_orthoSize = 5;

					bool m_isOrtho = false;

					Math::Vec3 m_worldPosition;
					Math::Vec3 m_worldRotation;

					// * Size of the rendered screen
					Math::Vec2 m_ratio = { 1.0f, 1.0f };

					// * Position in renderer
					int m_depth = 0;

					std::vector<Graphics::PostProcess::PostProcess *>* m_postProcess;

					Graphics::FrameBuffer* m_framebuffer = nullptr;
					Graphics::CameraSpecificData* m_specificData = nullptr;

					float m_frustum[24];
				};

				struct comp_CameraData
				{
					inline bool operator()( const CameraData* struct1, const CameraData* struct2 ) const
					{
						return ( struct1->GetDepth() < struct2->GetDepth() );
					}
				};
			}
		}
	}
}
