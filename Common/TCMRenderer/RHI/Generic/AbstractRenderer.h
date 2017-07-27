#pragma once

#include "MathLib/Mat4.h"
#include <vector>
#include "RenderData.h"

#define HDR_ENABLED false

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec2;
		}
	}
}

namespace TCM
{
	namespace Debug
	{
		struct GizmoMesh;
	}
}

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace PostProcess
			{
				struct PostProcess;
			}

			typedef struct s_TwoMatricesBlock
			{
				uint32_t size = sizeof(Math::Mat4) * 2;
				// View or World matrix
				Math::Mat4 first;
				// Projection or Transformation matrix
				Math::Mat4 second;
			} TwoMatricesBlock;

			/**
			 * \brief Abstract class auto-implemented used for Rendering
			 */
			class AbstractRenderer abstract
			{
			public:
				virtual ~AbstractRenderer();

				/**
				 * \brief Initialize render engine
				 * \param width _IN_ Size X of the window
				 * \param height _IN_ Size Y of the window
				 */
				static void Init( const int width, const int height );

				/**
				 * \brief Shutdowns renderer
				 */
				static void Shutdown();

				/**
				 * \brief Render on the CameraData RenderData with influents TCNLightData
				 * \param camera _IN_ Camera to render to
				 * \param queueRenderData _IN_ Object of the scene
				 * \param influentLights _IN_ Lights of the scene
				 */
				static void Render( Camera::CameraData* camera, const std::vector<const RenderData*>& queueRenderData,
				                    const std::vector<s_LightData*>& influentLights );

				/**
				 * \brief Render on the CameraData the GizmoLine list
				 * \param camera _IN_ Camera to render to
				 * \param gizmoLines _IN_ List of lines
				 * \param gizmoLinesCount _IN_ Line count
				 */
				static void RenderGizmoLines( Camera::CameraData* camera, const std::vector<float>& gizmoLines, const int gizmoLinesCount );

				/**
				 * \brief Render on the CameraData the GizmoPoint list
				 * \param camera _IN_ Camera to render to
				 * \param gizmoPoints _IN_ List of points
				 * \param gizmoPointsCount _IN_ Point count
				 */
				static void RenderGizmoPoints( Camera::CameraData* camera, const std::vector<float>& gizmoPoints, const int gizmoPointsCount );

				/**
				 * \brief Render on the CameraData the GizmoMesh list
				 * \param camera _IN_ Camera to render to
				 * \param gizmoMesh _IN_ List of GizmoMesh
				 */
				static void RenderGizmoMesh( Camera::CameraData* camera, const Debug::GizmoMesh& gizmoMesh );

				/**
				* \brief Update render after displaying new frame
				*/
				static void PreRender();

				/**
				 * \brief Update render after displaying new frame
				 */
				static void PostRender();

				/**
				 * \brief Renders a texture on the screen
				 * \param tex _IN_ The Texture to be rendered
				 * \param shader _IN_OPT_ The Shader to use
				 */
				static void RenderScreen( s_AbstractTexture* tex, s_AbstractShader* shader = nullptr );

				/**
				* \brief Renders a sprite
				* \param camera _IN_ Camera to be rendered to
				* \param spriteList _IN_ The Textures to be rendered
				* \param isInCanvas _IN_OPT_ Should depend of camera position
				*/
				static void RenderSprites( Camera::CameraData* camera,
				                           std::vector<std::pair<const s_AbstractTexture*, Math::Mat4>> spriteList, bool isInCanvas = false );

				/**
				 * \brief Creates a s_FrameBuffer
				 * \return Frame Buffer created (send a nullptr if cannot be created)
				 */
				static s_FrameBuffer* CreateFrameBuffer();

				/**
				* \brief Creates specific data for camera
				* \return The s_CameraSpecificData that will be created (send a nullptr if cannot be created)
				*/
				static s_CameraSpecificData* CreateCameraSpecificData();

				static void UpdateCameraData( Camera::CameraData* camera );

				/**
				 * \brief Create specific s_FrameBuffer for rendering, used for CameraData
				 * \return Frame Buffer to be created (send a nullptr if cannot be created)
				 * \param size _IN_ Ratio of the texture
				 * \param count _IN_ Specific count
				 */
				static s_FrameBuffer* CreateRendererFrameBuffer( const Math::Vec2& size,
				                                                 FrameBufferType type, const int count );

				static void SetShadowParams( const int width, const int height, const float& near_plane, const float& far_plane );

				static void GetShadowParams( int& width, int& height, float& near_plane, float& far_plane );

				/**
				 * \brief Draw the skybox
				 * \param camera _IN_ Camera to render to
				 * \param skybox _IN_ Skybox used
				 */
				static void DrawSkybox( Camera::CameraData* camera, const s_AbstractSkybox* skybox );

				/**
				 * \brief Get the ViewPort size
				 * \param width _OUT_ Size X of the ViewPort
				 * \param height _OUT_ Size Y of the ViewPort
				 */
				static void GetScreenSize( int& width, int& height );

				/**
				 * \brief Create projection matrix using current width and height
				 * \param ratio _IN_ Ratio of the screen size
				 * \param fov _IN_ Fov
				 * \param near _IN_ Near distance
				 * \param far _IN_ Far distance
				 * \return Projection matrix
				 */
				static Math::Mat4 GenProjMatrix( const Math::Vec2& ratio, const float fov, const float near, const float far );

				/**
				* \brief Create orthographic matrix using current width and height
				* \param ratio _IN_ Ratio of the screen size
				* \param near _IN_ Near distance
				* \param far _IN_ Far distance
				* \return Orhtographic matrix
				*/
				static Math::Mat4 GenOrthoMatrix( const Math::Vec2& ratio, const float near, const float far );

				/**
				 * \brief Create orthographic matrix using current width and height
				 * \param ratio _IN_ Ratio of the screen size
				 * \return Orhtographic matrix
				 */
				static Math::Mat4 GenOrthoMatrix( const Math::Vec2& ratio );

				/**
				 * \brief Paste the s_FrameBuffer
				 * \param s_FrameBuffer _IN_ s_FrameBuffer to paste
				 * \param position _IN_ Position to paste the s_FrameBuffer
				 * \param size _IN_ Size/ratio of the buffer
				 */
				static void PasteBufferOnScreen( const s_FrameBuffer* s_FrameBuffer, const Math::Vec2& position, const Math::Vec2& size );

				/**
				 * \brief Render the PostProcessEffect list of the CameraData
				 * \param camera _IN_ Camera to render to
				 */
				static void PostProcess( Camera::CameraData* camera );

				/**
				 * \brief Clear the screen
				 */
				static void Clear();

				/**
				 * \brief Get the comparer for the RenderData list
				 * \return RenderDataComparer used to sort RenderData
				 */
				static RenderDataComparer GetComparer();

				/**
				 * \brief Callback for screen resizing
				 * \param width _IN_ New width
				 * \param height _IN_ New height
				 */
				static void ResizeRenderer( const int width, const int height );

#pragma region Factory calls
			public:
				/**
				 * \brief Creates a RenderData of the type needed for the actual API
				 * \return The new well-typed RenderData*
				 */
				static RenderData* CreateRenderData();

				/**
				 * \brief Creates a TCM::Engine::Graphics::s_AbstractTexture of the type needed for the actual API
				 * \return The new well-typed TCM::Engine::Graphics::s_AbstractTexture*
				 */
				static s_AbstractTexture* CreateAbstractTexture();

				/**
				 * \brief Creates a new well-typed Mesh from vertices and indices
				 * \return The Mesh that will be created (send a nullptr if cannot be created)
				 * \param vertexNum _IN_ The number of vertices in the vertexBuff
				 * \param vertexBuff _IN_ The vertices buffer to be sent to the API
				 * \param indicesNum _IN_ The number of indices in the indicesBuff
				 * \param indicesBuff _IN_ The indices buffer to be sent to the API
				 */
				static s_AbstractMesh* CreateMesh(
					const uint32_t& vertexNum, const float* vertexBuff,
					const uint32_t& indicesNum, const uint16_t* indicesBuff );

				/**
				 * \brief Creates a new well-typed Texture from baseTexture
				 * \return The Texture that will be created (send a nullptr if cannot be created)
				 */
				static s_AbstractTexture* CreateTexture( const int type, const int width, const int height, const uint8_t* texData );

				/**
				 * \brief Creates a new well-typed Material from baseMaterial
				 * \return The Material that will be created (send a nullptr if cannot be created)
				 */
				static AbstractMaterial* CreateMaterial();

				/**
				 * \brief Create a new well-typed Shader from shader files
				 * \return The Shader that will be created (send a nullptr if cannot be created)
				 * \param vertexShader _IN_ The path to the wanted Vertex Shader
				 * \param fragmentShader _IN_ The path to the wanted Fragment Shader
				 * \param geometryShader _IN_ The path to the wanted Geometry Shader (optionnal)
				 */
				static s_AbstractShader* CreateShader(
					const char* vertexShader, const char* fragmentShader,
					const char* geometryShader = nullptr, const bool = false );

				/**
				 * \brief Creates a new well-typed Light with a new id 
				 * \return The Light that will be created (send a nullptr if cannot be created)

				 */
				static s_LightData* CreateLight();

				/**
				 * \brief Creates a new well-typed Skybox from different files of different faces
				 * \return The Skybox that will be created (send a nullptr if cannot be created)
				 * \param skyfaces _IN_ The paths of every face
				 */
				static s_AbstractSkybox* CreateSkybox(
					std::string skyfaces[6] );

			protected:
				/**
				 * \brief Creates a TCM::Engine::Graphics::s_AbstractTexture of the type needed for the actual API
				 * \return The new well-typed TCM::Engine::Graphics::s_AbstractTexture*
				 */
				virtual s_AbstractTexture* CoreCreateAbstractTexture() = 0;

				/**
				 * \brief Creates a new well-typed Mesh from vertices and indices
				 * \return The Mesh that will be created (send a nullptr if cannot be created)
				 * \param vertexNum _IN_ The number of vertices in the vertexBuff
				 * \param vertexBuff _IN_ The vertices buffer to be sent to the API
				 * \param indicesNum _IN_ The number of indices in the indicesBuff
				 * \param indicesBuff _IN_ The indices buffer to be sent to the API
				 */
				virtual s_AbstractMesh* CoreCreateMesh(
					const uint32_t& vertexNum, const float* vertexBuff,
					const uint32_t& indicesNum, const uint16_t* indicesBuff ) = 0;

				/**
				 * \brief Creates a new well-typed Texture from baseTexture
				 * \return The Texture created (send a nullptr if cannot be created)
				 * \param type _IN_ The Type of the texture.
				 * \param width _IN_ The Width of the texture.
				 * \param height _IN_ The Height of the texture.
				 * \param texData _IN_ The Data of the texture.
				 */
				virtual s_AbstractTexture* CoreCreateTexture( const int type, const int width, const int height, const uint8_t* texData ) = 0;

				/**
				 * \brief Creates a new well-typed Material from baseMaterial
				 * \return The Material that will be created (send a nullptr if cannot be created)
				 */
				virtual AbstractMaterial* CoreCreateMaterial() = 0;

				/**
				 * \brief Create a new well-typed Shader from shader files
				 * \return The Shader that will be created (send a nullptr if cannot be created)
				 * \param vertexShader _IN_ The path to the wanted Vertex Shader
				 * \param fragmentShader _IN_ The path to the wanted Fragment Shader
				 * \param geometryShader _IN_ The path to the wanted Geometry Shader (optionnal)
				 */
				virtual s_AbstractShader* CoreCreateShader(
					const char* vertexShader, const char* fragmentShader,
					const char* geometryShader, const bool deferred ) = 0;

				/**
				 * \brief Creates a new well-typed Light with a new id
				 * \return The Light that will be created (send a nullptr if cannot be created)
				 */
				virtual s_LightData* CoreCreateLight() = 0;

				/**
				 * \brief Creates a new well-typed Skybox from different files of different faces
				 * \return The Skybox that will be created (send a nullptr if cannot be created)
				 * \param skyfaces _IN_ The paths of every face
				 */
				virtual s_AbstractSkybox* CoreCreateSkybox( std::string skyfaces[6] ) = 0;
#pragma endregion

				AbstractRenderer() = default;

				/**
				 * \brief Initialize render engine
				 * \param width _IN_ Size X of the window
				 * \param height _IN_ Size Y of the window
				 */
				virtual void CoreInitializeRenderer( const int width, const int height ) = 0;

				/**
				 * \brief Shutdowns renderer
				 */
				virtual void CoreShutdown() = 0;

				/**
				 * \brief Clear buffer
				 */
				virtual void CoreClear() = 0;

				/**
				 * \brief Updates Render
				 */
				virtual void CoreRender( Camera::CameraData* camera, const std::vector<const RenderData*>& queueRenderData,
				                         const std::vector<s_LightData*>& influentLights ) = 0;

				virtual void CoreRenderGizmoLines( Camera::CameraData* camera, const std::vector<float>& gizmoLines, const int gizmoLinesCount ) = 0;

				virtual void CoreRenderGizmoPoints( Camera::CameraData* camera, const std::vector<float>& gizmoPoints, const int gizmoPointsCount ) = 0;

				virtual void CoreRenderGizmoMesh( Camera::CameraData* camera, const Debug::GizmoMesh& gizmoMesh ) = 0;

				// * Update render before displaying new frame
				virtual void CorePreRender()
				{
					CoreClear();
				};

				// * Update render before displaying new frame
				virtual void CorePostRender()
				{
				};

				void CoreDrawRenderData( Camera::CameraData* camera, const RenderData* renderData,
				                         s_AbstractShader* forcedShader );

				virtual void CoreDrawRenderData( Camera::CameraData* camera, const RenderData* renderData,
				                                 s_LightData* light = nullptr, s_AbstractShader* forcedShader = nullptr ) = 0;

				/**
				 * \brief Draws a skybox
				 */
				virtual void CoreDrawSkybox( Camera::CameraData* camera, const s_AbstractSkybox* skybox ) = 0;

				/**
				 * \brief Renders a texture on the screen
				 * \param tex _IN_ The Texture to be rendered
				 * \param shader _IN_OPT_ The Shader to use
				 */
				virtual void CoreRenderScreen( const s_AbstractTexture* tex, AbstractShader* shader = nullptr ) = 0;

				/**
				* \brief Renders a sprite list
				* \param camera _IN_ Camera to be rendered to
				* \param spriteList _IN_ The Textures to be rendered
				* \param isInCanvas _IN_OPT_ Should depend of camera position
				*/
				void CoreRenderSprites( Camera::CameraData* camera,
				                        std::vector<std::pair<const s_AbstractTexture*, Math::Mat4>> spriteList, bool isInCanvas = false );

				/**
				* \brief Renders a sprite
				* \param camera _IN_ Camera to be rendered to
				* \param sprite _IN_ The Texture to be rendered
				* \param isInCanvas _IN_OPT_ Should depend of camera position
				*/
				virtual void CoreRenderSprite( Camera::CameraData* camera,
				                               std::pair<const s_AbstractTexture*, Math::Mat4> sprite, bool isInCanvas = false ) = 0;

				/**
				 * \brief Creates a s_FrameBuffer
				 * \return Frame Buffer to be created (send a nullptr if cannot be created)
				 */
				virtual s_FrameBuffer* CoreCreateFrameBuffer() = 0;

				/**
				* \brief Creates specific data for camera
				* \return The s_CameraSpecificData that will be created (send a nullptr if cannot be created)
				*/
				virtual s_CameraSpecificData* CoreCreateCameraSpecificData() = 0;

				virtual void CoreUpdateCameraData( Camera::CameraData* camera ) = 0;

				virtual s_FrameBuffer* CoreCreateRendererFrameBuffer( const Math::Vec2& size, FrameBufferType type, const int count ) = 0;

				/**
				* \brief Get the ViewPort size
				* \param width _OUT_ Size X of the ViewPort
				* \param height _OUT_ Size Y of the ViewPort
				*/
				void CoreGetScreenSize( int& width, int& height ) const;

				Math::Mat4 CoreGenProjMatrix( const Math::Vec2& ratio, const float fov, const float near, const float far ) const;

				Math::Mat4 CoreGenOrthoMatrix( const Math::Vec2& ratio, const float near, const float far ) const;

				Math::Mat4 CoreGenOrthoMatrix( const Math::Vec2& ratio ) const;

				/**
				* \brief Paste the s_FrameBuffer
				* \param s_FrameBuffer _IN_ s_FrameBuffer to paste
				* \param position _IN_ Position to paste the s_FrameBuffer
				* \param size _IN_ Size/ratio of the buffer
				*/
				virtual void CorePasteBufferOnScreen( const s_FrameBuffer* s_FrameBuffer, const Math::Vec2& position, const Math::Vec2& size ) = 0;

				RenderDataComparer CoreGetComparer() const;

				virtual void CoreResizeRenderer( const int width, const int height ) = 0;

			private:
				/**
				 * \brief Get the unique instance of the AbstractRenderer
				 * \return A reference to the AbstractRenderer
				 */
				static AbstractRenderer& GetInstance();

			protected:

				virtual void CorePostProcess( Camera::CameraData* camera ) = 0;

				/**
				 * \brief Draws the Post Process
				 * \param from _IN_ s_FrameBuffer on which the post process takes effect
				 */
				void GenericPostProcess( s_FrameBuffer* from, std::vector<PostProcess::PostProcess*>* postProcesses );

				/**
				* \brief Resize viewport only if necessary
				* \param width _IN_ New width of viewport
				* \param height _IN_ New height of viewport
				*/
				void ResizeViewPort( const int width, const int height );

				/**
				 * \brief Resize viewport only if necessary
				 * \param width _IN_ New width of viewport
				 * \param height _IN_ New height of viewport
				 */
				virtual void CoreResizeViewPort( const int width, const int height ) = 0;

				/**
				 * \brief Bind shader if only necessary
				 * \param shader _IN_ Shader to bind
				 */
				void CoreBindShader( s_AbstractShader* shader );

				virtual void CoreInitSkybox() = 0;
				/**
				 * \brief The s_FrameBuffer used for Post Processing
				 */
				s_FrameBuffer* m_pingPongBuffers[2];

				s_AbstractShader* m_lastShader = nullptr;

				s_AbstractShader* m_skyboxShader = nullptr;

				int m_width;
				int m_height;

				int m_shadowMapSizeX = 1024;
				int m_shadowMapSizeY = 1024;
				float m_nearPlane = 1.0f;
				float m_farPlane = 7.5f;

				int m_viewportSizeX;
				int m_viewportSizeY;
			};
		}
	}
}
