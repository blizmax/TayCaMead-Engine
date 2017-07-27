#pragma once

#include "TCMRenderer/RendererDefines.h"

#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "TCMRenderer/Camera/CameraManager.h"

struct TCMCameraData;

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			class RendererFactory final
			{
			public:
				/**
				 * \brief Creates a RenderData of the type needed for the actual API
				 * \return The new well-typed RenderData*
				 */
				static TCMRENDERER_DLL RenderData* CreateRenderData();

				/**
				 * \brief Creates a TCM::Engine::Graphics::AbstractTexture of the type needed for the actual API
				 * \return The new well-typed TCM::Engine::Graphics::AbstractTexture*
				 */
				static TCMRENDERER_DLL AbstractTexture* CreateAbstractTexture();

				/**
				 * \brief Creates a new well-typed Mesh from vertices and indices
				 * \param vertexNum _IN_ The number of vertices in the vertexBuff
				 * \param vertexBuff _IN_ The vertices buffer to be sent to the API
				 * \param indicesNum _IN_ The number of indices in the indicesBuff
				 * \param indicesBuff _IN_ The indices buffer to be sent to the API
				 */
				static TCMRENDERER_DLL AbstractMesh* CreateMesh(
					const uint32_t& vertexNum, const float* vertexBuff,
					const uint32_t& indicesNum, const uint16_t* indicesBuff );

				/**
				* \brief Creates a new well-typed Mesh from vertices and indices
				* \param vecFragmentVertex _IN_ The fragment vertex list required for the mesh.
				* \param vecFragment _IN_ The fragment list required for the mesh.
				* \return The Mesh created.
				*/
				static TCMRENDERER_DLL AbstractMesh* CreateMesh(
					std::vector<VertexData*>& vecFragmentVertex,
					std::vector<Fragment*>& vecFragment );

				/**
				 * \brief Creates a new well-typed Texture from baseTexture
				 * \param type _IN_ The Type of the Texture
				 * \param width _IN_ The Width of the Texture
				 * \param height _IN_ The Height of the Texture
				 * \param texData _IN_ The Data of the Texture
				 * \return The Texture that will be created (send a nullptr if cannot be created)
				 */
				static TCMRENDERER_DLL AbstractTexture* CreateTexture( const int type, const int width, const int height, const uint8_t* texData );

				/**
				 * \brief Creates a new well-typed Material from baseMaterial
				 */
				static TCMRENDERER_DLL AbstractMaterial* CreateMaterial();

				/**
				 * \brief Create a new well-typed Shader from shader files
				 * \param vertexShader _IN_ The path to the wanted Vertex Shader
				 * \param fragmentShader _IN_ The path to the wanted Fragment Shader
				 * \param geometryShader _IN_ The path to the wanted Geometry Shader (optionnal)
				 */
				static TCMRENDERER_DLL AbstractShader* CreateShader( const char* vertexShader, const char* fragmentShader,
				                                                     const char* geometryShader = nullptr, const bool deferred = false );

				/**
				 * \brief Creates a new well-typed Light with a new id 
				 * \return The Light that will be created (send a nullptr if cannot be created)
				 */
				static TCMRENDERER_DLL LightData* CreateLight();

				static TCMRENDERER_DLL void SetShadowParams( const int width, const int height, const float& near_plane, const float& far_plane );

				static TCMRENDERER_DLL void GetShadowParams( int& width, int& height, float& near_plane, float& far_plane );
				/**
				 * \brief Creates a new well-typed Skybox from different files of different faces
				 * \return The Skybox that will be created (send a nullptr if cannot be created)
				 * \param skyfaces _IN_ The paths of every face
				 */
				static TCMRENDERER_DLL AbstractSkybox* CreateSkybox( std::string skyfaces[6] );

				/**
				* \brief Creates a new well-typed framebuffer
				* \return The framebuffer that will be created (send a nullptr if cannot be created)
				*/
				static TCMRENDERER_DLL FrameBuffer* CreateFramebuffer();

				/**
				* \brief Creates specific data for camera
				* \return The CameraSpecificData that will be created (send a nullptr if cannot be created)
				*/
				static TCMRENDERER_DLL CameraSpecificData* CreateCameraSpecificData();

				static TCMRENDERER_DLL void UpdateCameraData( Camera::CameraData* camera );

				/**
				* \brief Creates a new well-typed framebuffer used to render
				* \return The framebuffer that will be created (send a nullptr if cannot be created)
				* \param size _IN_ Relative size of the framebuffer
				* \param type _IN_ The type of Frame Buffer
				* \param count _IN_ The number of frame buffer
				*/
				static TCMRENDERER_DLL FrameBuffer* CreateRendererFramebuffer( const Math::Vec2& size, FrameBufferType type, const int count = 1 );

				static TCMRENDERER_DLL Math::Mat4 GenProjMatrix( const Math::Vec2& ratio, float fov, float near, float far );

				static TCMRENDERER_DLL Math::Mat4 GenOrthoMatrix( const Math::Vec2& ratio );

				static TCMRENDERER_DLL Math::Mat4 GenOrthoMatrix( const Math::Vec2& ratio, float near, float far );

				static TCMRENDERER_DLL void Init( const int width, const int height );

				static TCMRENDERER_DLL void Resize( const int width, const int height );
			};
		}
	}
}
