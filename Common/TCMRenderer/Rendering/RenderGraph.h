#pragma once

#include "TCMRenderer/RendererDefines.h"
#include "TCMRenderer/Gizmo/GizmoMesh.h"
#include "MathLib/Mat4.h"
#include <vector>

struct TCMCameraData;

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec2;
		}
	}

	namespace Debug
	{
		struct GizmoMesh;
		struct GizmoPoint;
		struct GizmoLine;
	}

	namespace Engine
	{
		namespace Graphics
		{
			struct s_FrameBuffer;
			struct s_LightData;
			struct s_AbstractSkybox;
			struct s_AbstractTexture;
			class RenderableObject;
			class SpriteData;
			class RenderData;

			namespace Camera
			{
				struct CameraData;
			}

			/**
			 * \brief Class used for managing Render outside TCMRenderer
			 */
			class RenderGraph final
			{
			public:
				~RenderGraph();

				/**
				 * \brief Prepare scene for rendering
				 * \param scene _IN_ Scene to render
				 */
				static void PreRenderScene( RenderableObject* scene );
				/**
				 * \brief Renders a scene and everything that it contains
				 * * \param camera _IN_ Target camera
				 */
				static void RenderScene( Camera::CameraData* camera );

				static void PreRender();

				static TCMRENDERER_DLL void PostRender();

				/**
				 * \brief Registers a RenderData to be drawn when rendering
				 * \param renderData _IN_ The RenderData to be drawn
				 */
				static TCMRENDERER_DLL void RegisterRenderData( const RenderData* renderData );

				/**
				* \brief Unregisters a RenderData
				* \param renderData _IN_ The RenderData to be unregistered
				*/
				static TCMRENDERER_DLL void UnregisterRenderData( const RenderData* renderData );

				/**
				* \brief Registers a TCM::Engine::Graphics::SpriteData to be drawn when rendering
				* \param texture _IN_ The TCM::Engine::Graphics::AbstractTexture to be drawn
				*/
				static TCMRENDERER_DLL void RegisterSprite( const SpriteData* texture );

				/**
				* \brief Unregisters a TCM::Engine::Graphics::SpriteData
				* \param texture _IN_ The TCM::Engine::Graphics::AbstractTexture to be unregister
				*/
				static TCMRENDERER_DLL void UnregisterSprite( const SpriteData* texture );

				/**
				* \brief Registers a RenderData to be drawn when rendering
				* \param texture _IN_ The TCM::Engine::Graphics::AbstractTexture to be drawn
				* \param worldMatrix _IN_ The World Matrix of the RenderData being drawn
				*/
				static TCMRENDERER_DLL void RegisterUISprite( const s_AbstractTexture* texture, const Math::Mat4& worldMatrix );

				/**
				 * \brief Registers a skybox to be drawn when rendering
				 * \param skyboxData _IN_ The skybox to be drawn
				 */
				static TCMRENDERER_DLL void RegisterSkybox( s_AbstractSkybox* skyboxData );

				static TCMRENDERER_DLL void RegisterGizmoLine( const Debug::GizmoLine& lines );

				static TCMRENDERER_DLL void RegisterGizmoPoint( const Debug::GizmoPoint& point );

				static TCMRENDERER_DLL void RegisterGizmoMesh( const Debug::GizmoMesh& mesh );

				/**
				 * \brief Registers a light to influent lights
				 * \param data _IN_ Light to be registered
				 */
				static TCMRENDERER_DLL void RegisterLight( s_LightData* data );

				/**
				* \brief Unregisters a light
				* \param data _IN_ Light to be unregistered
				*/
				static TCMRENDERER_DLL void UnregisterLight( s_LightData* data );

				/**
				* \brief Paste the FrameBuffer
				* \param frameBuffer _IN_ Framebuffer to paste
				* \param position _IN_ Position to paste the framebuffer
				* \param size _IN_ Size/ratio of the buffer
				*/
				static void PasteBufferOnScreen( s_FrameBuffer* frameBuffer, const Math::Vec2& position, const Math::Vec2& size );

				static TCMRENDERER_DLL void UnregisterAll();

				static const RenderData* SphereRaycast( const Math::Vec3& origin, const Math::Vec3& direction );

			protected:
				RenderGraph();

				void CorePreRenderScene( RenderableObject* scene ) const;
				/**
				 * \brief Renders a scene and everything that it contains
				 * * \param camera _IN_ Target camera
				 */
				void CoreRenderScene( Camera::CameraData* camera );

				void CorePreRender() const;

				void CorePostRender();

				/**
				 * \brief Registers a RenderData to be drawn when rendering
				 * \param renderData _IN_ The RenderData to be drawn
				 */
				void CoreRegisterRenderData( const RenderData* renderData );

				/**
				* \brief Registers a RenderData to be drawn when rendering
				* \param renderData _IN_ The RenderData to be drawn
				*/
				void CoreUnregisterRenderData( const RenderData* renderData );

				/**
				* \brief Registers a TCM::Engine::Graphics::SpriteData to be drawn when rendering
				* \param texture _IN_ The TCM::Engine::Graphics::SpriteData to be drawn
				*/
				void CoreRegisterSprite( const SpriteData* texture );

				/**
				* \brief Unegisters a TCM::Engine::Graphics::SpriteData to be drawn when rendering
				* \param texture _IN_ The TCM::Engine::Graphics::SpriteData to be unregistered
				*/
				void CoreUnregisterSprite( const SpriteData* texture );

				/**
				* \brief Registers a TCM::Engine::Graphics::AbstractTexture to be drawn when rendering
				* \param texture _IN_ The TCM::Engine::Graphics::AbstractTexture to be drawn
				* \param worldMatrix _IN_ The World Matrix of the RenderData being drawn
				*/
				void CoreRegisterUISprite( const s_AbstractTexture* texture, const Math::Mat4& worldMatrix );

				/**
				* \brief Registers a skybox to be drawn when rendering
				* \param skyboxData _IN_ The skybox to be drawn
				*/
				void CoreRegisterSkybox( s_AbstractSkybox* skyboxData );

				void CoreRegisterGizmoLine( const Debug::GizmoLine& lines );

				void CoreRegisterGizmoPoint( const Debug::GizmoPoint& point );

				void CoreRegisterGizmoPoint( const Debug::GizmoPoint& point, std::vector<float>& list ) const;

				void CoreRegisterGizmoMesh( const Debug::GizmoMesh& mesh );

				void CoreUnregisterGizmos();
				/**
				 * \brief Registers a light to influent lights
				 * \param data _IN_ Light to be registered
				 */
				void CoreRegisterLight( s_LightData* data );

				/**
				* \brief Unregisters a light
				* \param data _IN_ Light to be unregistered
				*/
				void CoreUnregisterLight( s_LightData* data );

				/**
				* \brief Paste the FrameBuffer
				* \param frameBuffer _IN_ Framebuffer to paste
				* \param position _IN_ Position to paste the framebuffer
				* \param size _IN_ Size/ratio of the buffer
				*/
				void CorePasteBufferOnScreen( s_FrameBuffer* frameBuffer, const Math::Vec2& position, const Math::Vec2& size ) const;

				void CoreUnregisterAll();

				const RenderData* CoreSphereRaycast( const Math::Vec3& origin, const Math::Vec3& direction );

			private:
				/**
				 * \brief Get the unique instance of the RenderGraph
				 * \return A reference to the RenderGraph
				 */
				static RenderGraph& GetInstance();

				/**
				 * \brief The priority queue for every renderable object
				 */
				std::vector<const RenderData *> m_renderDataList;

				/**
				* \brief The priority queue for every renderable object
				*/
				std::vector<const SpriteData *> m_spriteList;

				/**
				* \brief The priority queue for every renderable object
				*/
				std::vector<std::pair<const s_AbstractTexture *, Math::Mat4>> m_uiSpriteList;

				/**
				* \brief Vector of every influent light of the scene
				*/
				std::vector<s_LightData *> m_lightList;

				std::vector<s_LightData *> m_visibleLights;
				std::vector<const RenderData *> m_visibleRenderData;

				s_AbstractSkybox* m_currentSkybox;

				std::vector<float> m_gizmoPoints;
				int m_gizmoPointsCount = 0;

				std::vector<float> m_gizmoLines;
				int m_gizmoLinesCount = 0;

				std::vector<Debug::GizmoMesh> m_gizmoMeshes;
			};
		}
	}
}
