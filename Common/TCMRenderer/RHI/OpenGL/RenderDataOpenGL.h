#pragma once

#include "TCMRenderer/RHI/Generic/RenderData.h"

#include "OpenGLLibs.h"
#include "MathLib/Mat4.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace OpenGL
			{
				/**
				 * \brief Structure containing the data needed for one Mesh on OpenGL
				 */
				struct MeshOpenGL : AbstractMesh
				{
					MeshOpenGL( const uint32_t& vertexNum, const float* vertexBuff,
					            const uint32_t& indicesNum, const uint16_t* indicesBuff );
					virtual ~MeshOpenGL();

					/**
					 * \brief ID of the VAO of the object
					 */
					GLuint m_idVAO = 0;

					/**
					 * \brief ID of the VBO of the object
					 */
					GLuint m_idVBO = 0;

					/**
					 * \brief ID of the IBO of the object
					 */
					GLuint m_idIBO = 0;
				};

				/**
				 * \brief Structure containing the data needed for one Texture on OpenGL
				 */
				struct TextureOpenGL : AbstractTexture
				{
					TextureOpenGL();

					TextureOpenGL( const int type, const int width, const int height, const uint8_t* texData );

					virtual ~TextureOpenGL();
					/**
					 * \brief ID of the texture of the object
					 */
					GLuint m_idTex = 0;
				};

				/**
				 * \brief Structure containing the data needed for one Material on OpenGL
				 */
				struct MaterialOpenGL : AbstractMaterial
				{
					MaterialOpenGL();
					virtual ~MaterialOpenGL();

					void UpdateData() override;

					GLuint GetIdUbo() const;

				protected:
					/**
					 * \brief ID of the UBO of the Material
					 */
					GLuint m_idUBO = 0;
				};

				struct FrameBufferGL : FrameBuffer
				{
					FrameBufferGL( FrameBufferType type = FrameBufferType::NONE );
					FrameBufferGL( FrameBufferType type, const int width, const int height, int count = 1 );
					~FrameBufferGL();

					void BindBuffer() const override;
					void UnbindBuffer() override;
					void ClearBuffer( Math::ColorRGB color = { 1.0f, 0.0f, 0.0f } ) override;
					void CoreResize( const int width, const int height ) const override;

					AbstractTexture* GenTexture( TextureType type, int width, int height ) override;

					AbstractTexture* ResizeTexture( TextureType type, int width, int height ) override;

					// * Attach texture to framebuffer
					void AttachTexture( TextureType type, AbstractTexture* texture ) override;

					void AttachTexture( TextureType type, int typeTexture, AbstractTexture* texture );

					void LinkTexture( TextureType type, int typeTexture, AbstractTexture* texture ) const;

					int GetID() const override;

					GLuint m_optDepthFBO;

				protected:
					int ConvertType( TextureType t ) const override;

				private:
					uint32_t m_FBO;
				};

				struct CameraSpecificDataGL : CameraSpecificData
				{
					CameraSpecificDataGL();
					~CameraSpecificDataGL();

					void UpdateData( Camera::CameraData* data ) override;
					GLuint m_UBO = 0;
				};

				/**
				 * \brief Structure containing the data needed for one Light on OpenGL
				 */
				struct LightDataGL : LightData
				{
					LightDataGL( const int width, const int height );
					virtual ~LightDataGL();

					void UpdateData() override;

					/**
					 * \brief ID of the UBO of the light
					 */
					GLuint m_idUBO = 0;
				};

				class ShaderOpenGL;

				/**
				 * \brief Structure containing the data needed for one Skybox on OpenGL
				 */
				struct SkyboxOpenGL : AbstractSkybox
				{
					SkyboxOpenGL() = default;
					virtual ~SkyboxOpenGL();

					/**
					 * \brief ID of the cubemap texture of the Skybox
					 */
					uint32_t m_idCubemapTex;
				};

				/**
				 * \brief RenderData and Mat4 comparer for priority queue
				 */
				struct RenderDataComparerOpenGL : RenderDataComparer
				{
					virtual ~RenderDataComparerOpenGL() = default;

					bool operator()( const RenderData* dataLeft, const RenderData* dataRight ) const override;
				};
			}
		}
	}
}
