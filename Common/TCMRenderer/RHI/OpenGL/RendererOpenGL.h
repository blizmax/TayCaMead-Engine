#pragma once

#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"

#include "OpenGLLibs.h"
#include <queue>
#include "ShaderOpenGL.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace OpenGL
			{
				struct FrameBufferGL;
				struct TextureOpenGL;

				/**
				 * \brief Implemented AbstractRenderer using the OpenGL API
				 */
				class RendererOpenGL : public AbstractRenderer
				{
				public:
					RendererOpenGL();

					virtual ~RendererOpenGL();

				protected:
					/**
					 * \brief Initialize render engine
					 * \param width _IN_ Size X of the window
					 * \param height _IN_ Size Y of the window
					 */
					void CoreInitializeRenderer( const int width, const int height ) override;

					/**
					 * \brief Shutdowns renderer
					 */
					void CoreShutdown() override;

					/**
					 * \brief Updates render before displaying new frame
					 */
					void CoreClear() override;

					/**
					 * \brief Updates Render
					 */
					void CoreRender( Camera::CameraData* camera, const std::vector<const RenderData*>& queueRenderData,
					                 const std::vector<LightData*>& influentLights ) override;

					/**
					 * \brief Draw something from its RenderData
					 * \param camera _IN_ Camera to render to
					 * \param renderData _IN_ The data of the object to be draw
					 * \param light _IN_OPT_ Influent light used
					 * \param forcedShader _IN_OPT_ The Shader forced to be used
					 */
					void CoreDrawRenderData( Camera::CameraData* camera, const RenderData* renderData,
					                         LightData* light = nullptr, AbstractShader* forcedShader = nullptr ) override;

					/**
					 * \brief Draws registered skybox
					 */
					void CoreDrawSkybox( Camera::CameraData* camera, const AbstractSkybox* skybox ) override;

					/**
					 * \brief Renders a texture on the screen
					 * \param tex _IN_ The Texture to be rendered
					 * \param shader _IN_OPT_ The Shader to use
					 */
					void CoreRenderScreen( const AbstractTexture* tex, AbstractShader* shader ) override;

					/**
					 * \brief Creates a FrameBuffer
					 * \return Frame Buffer to be created (send a nullptr if cannot be created)
					 */
					FrameBuffer* CoreCreateFrameBuffer() override;

					/**
					* \brief Creates specific data for camera
					* \return The CameraSpecificData that will be created (send a nullptr if cannot be created)
					*/
					CameraSpecificData* CoreCreateCameraSpecificData() override;

					void CoreUpdateCameraData( Camera::CameraData* camera ) override;

					// * Create specific buffer
					FrameBuffer* CoreCreateRendererFrameBuffer( const Math::Vec2& size, FrameBufferType type, const int count ) override;

					void CorePasteBufferOnScreen( const FrameBuffer* frameBuffer, const Math::Vec2& position, const Math::Vec2& size ) override;

#pragma region Factory calls
					/**
					 * \brief Creates a TCM::Engine::Graphics::AbstractTexture of the type needed for the actual API
					 * \return The new well-typed TCM::Engine::Graphics::AbstractTexture*
					 */
					AbstractTexture* CoreCreateAbstractTexture() override;

					/**
					 * \brief Creates a new well-typed Mesh from vertices and indices
					 * \return The Mesh that will be created (send a nullptr if cannot be created)
					 * \param vertexNum _IN_ The number of vertices in the vertexBuff
					 * \param vertexBuff _IN_ The vertices buffer to be sent to the API
					 * \param indicesNum _IN_ The number of indices in the indicesBuff
					 * \param indicesBuff _IN_ The indices buffer to be sent to the API
					 */
					AbstractMesh* CoreCreateMesh(
						const uint32_t& vertexNum, const float* vertexBuff,
						const uint32_t& indicesNum, const uint16_t* indicesBuff ) override;

					/**
					* \brief Create a texture from datas
					* \param type _IN_ Type of the texture
					* \param width _IN_ Width of the texture
					* \param height _IN_ Height of the texture
					* \param texData _IN_ Data of the texture
					*/
					AbstractTexture* CoreCreateTexture( const int type, const int width, const int height, const uint8_t* texData ) override;

					/**
					 * \brief Creates a new well-typed Material from baseMaterial
					 * \return The Material that will be created (send a nullptr if cannot be created)
					 */
					AbstractMaterial* CoreCreateMaterial() override;

					/**
					 * \brief Create a new well-typed Shader from shader files
					 * \return The Shader that will be created (send a nullptr if cannot be created)
					 * \param vertexShader _IN_ The path to the wanted Vertex Shader
					 * \param fragmentShader _IN_ The path to the wanted Fragment Shader
					 * \param geometryShader _IN_ The path to the wanted Geometry Shader (optionnal)
					 * \param deferred _IN_ Should the shader use deferred rendering ? (optionnal)
					 */
					AbstractShader* CoreCreateShader(
						const char* vertexShader, const char* fragmentShader,
						const char* geometryShader = nullptr, const bool deferred = false ) override;

					/**
					 * \brief Creates a new well-typed Light with a new id 
					 * \return The Light that will be created (send a nullptr if cannot be created)
					 */
					LightData* CoreCreateLight() override;

					/**
					 * \brief Creates a new well-typed Skybox from different files of different faces
					 * \return The Skybox that will be created (send a nullptr if cannot be created)
					 * \param skyfaces _IN_ The paths of every face
					 */
					AbstractSkybox* CoreCreateSkybox( std::string skyfaces[6] ) override;

					void CoreResizeViewPort( const int width, const int height ) override;
#pragma endregion

				private:
					/**
					 * \brief Basic Shader used for primary rendering
					 */
					ShaderOpenGL m_basicShader;

					/**
					 * \brief Basic Shader used for setting shadow map
					 */
					ShaderOpenGL m_shadowShader;

					/**
					 * \brief Basic Shader used for printing on screen
					 */
					ShaderOpenGL m_screenShader;

					/**
					* \brief Basic Shader used for gizmo rendering
					*/
					ShaderOpenGL m_gizmoShader;

#pragma region LastRenderered data
					/**
					 * \brief Last mesh ID used when rendering
					 */
					GLuint m_lastVAO = 0;

					/**
					 * \brief Last Shader ID used when rendering
					 */
					GLuint m_lastShader = 0;

					/**
					 * \brief Last Material ID used when rendering
					 */
					GLuint m_lastMaterial = 0;
#pragma endregion

#pragma region Gizmo Buffers

					GLuint m_gizmoVAO = 0;

					GLuint m_gizmoVBO = 0;

#pragma endregion

#pragma region Sprite Buffers

					GLuint m_spriteVAO = 0;

					GLuint m_spriteVBO = 0;
#pragma endregion

#pragma region PostProcess buffers
					/**
					 * \brief FrameBuffer for anti-aliasing
					 */
					FrameBufferGL* m_antiAliasingBuffer;

					/**
					 * \brief ID of the Depth Buffer
					 */
					uint32_t m_depthFBO;
#pragma endregion

					GLuint m_skyboxVAO;
					GLuint m_skyboxVBO;

					/**
					 * \brief Init buffers needed
					 * \param width _IN_ Size X of the buffer
					 * \param height _IN_ Size Y of the buffer
					 */
					void InitBuffers( int width, int height );

					void InitSpriteRenderer();

					/**
					 * \brief Bind one Texture on one GL_TEXTURE
					 * \param texture _IN_ The Texture to bind
					 * \param textureIndex _IN_ The index of GL_TEXTURE to bind
					 */
					void BindOneTexture( const TextureOpenGL* texture, const int textureIndex ) const;

					/**
					 * \brief Loads a Cubemap using different files
					 * \param skyfaces _IN_ Each paths of a skybox
					 * \return The textureID of the new Cubemap
					 */
					uint32_t LoadCubemap( std::string skyfaces[6] ) const;

					void InitGizmoRendering();

					void CorePostProcess( Camera::CameraData* camera ) override;

					void CoreRenderGizmoLines( Camera::CameraData* camera, const std::vector<float>& gizmoLines, const int gizmoLinesCount ) override;

					void CoreRenderGizmoPoints( Camera::CameraData* camera, const std::vector<float>& gizmoPoints, const int gizmoPointsCount ) override;

					void CoreRenderGizmoMesh( Camera::CameraData* camera, const Debug::GizmoMesh& gizmoMesh ) override;

					void CoreResizeRenderer( const int width, const int height ) override;

					void CoreRenderSprite( Camera::CameraData* camera, std::pair<const AbstractTexture*, Math::Mat4> sprite, bool isInCanvas ) override;

					void CoreInitSkybox() override;
				};
			}
		}
	}
}
