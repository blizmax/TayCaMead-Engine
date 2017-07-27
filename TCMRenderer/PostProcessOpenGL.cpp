#include "TCMRenderer/PostProcess/OpenGL/PostProcessOpenGL.h"
#include "TCMLogger/Logger.h"

#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include "TCMRenderer/RHI/OpenGL/RenderDataOpenGL.h"
#include "TCMRenderer/RHI/OpenGL/ShaderOpenGL.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace PostProcess
			{
				namespace OpenGL
				{
					BloomEffectGL::BloomEffectGL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/additiveTex.frag";

						verBloomShaderStr = vertexShaderStr;
						fragBloomShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/bloom.frag";
					}

					AbstractTexture* BloomEffectGL::CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer )
					{
						if ( m_shader == nullptr )
						{
							Init();
							Graphics::OpenGL::ShaderOpenGL* shader = dynamic_cast<Graphics::OpenGL::ShaderOpenGL*>(m_shader);

							m_shader->Bind();
							glUniform1i( glGetUniformLocation( shader->Get(), "screenTexture" ), 0 );
							glUniform1i( glGetUniformLocation( shader->Get(), "bloomTexture" ), 1 );
						}
						if ( m_shader == nullptr )
						{
							TCMFAILURE("Shader not init");
							return nullptr;
						}

						buffer->BindBuffer();
						buffer->ClearBuffer();
						AbstractRenderer::RenderScreen( texture );

						m_bloomBuffer->BindBuffer();
						m_bloomBuffer->ClearBuffer();

						m_bloomShader->Bind();
						AbstractRenderer::RenderScreen( texture, m_bloomShader );

						int width, height;
						AbstractRenderer::GetScreenSize( width, height );

						for ( int i = 0; i < count; ++i )
						{
							FrameBuffer* frameBuffer = m_containerBuffers[i];

							glBindFramebuffer( GL_READ_FRAMEBUFFER, m_bloomBuffer->GetID() );
							glBindFramebuffer( GL_DRAW_FRAMEBUFFER, frameBuffer->GetID() );
							glBlitFramebuffer( 0, 0, width, height, 0, 0,
							                   frameBuffer->GetTexture()->GetWidth(), frameBuffer->GetTexture()->GetHeight(),
							                   GL_COLOR_BUFFER_BIT, GL_LINEAR );

							PostProcessManager::GetPostProcess( PostProcessEffect::GAUSS_BLUR )->RenderEffect( frameBuffer->GetTexture(), m_containerBuffer );

							Graphics::OpenGL::TextureOpenGL* bloomTex = dynamic_cast<Graphics::OpenGL::TextureOpenGL *>(m_containerBuffer->GetTexture());

							buffer->BindBuffer();
							m_shader->Bind();

							glActiveTexture( GL_TEXTURE1 );
							glBindTexture( GL_TEXTURE_2D, bloomTex->m_idTex );
							glUniform1f( weightLocation, weight );

							AbstractRenderer::RenderScreen( texture, m_shader );
						}

						return buffer->GetTexture();
					}

					ToneMappingGL::ToneMappingGL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/toneMapping.frag";
					}

					AbstractTexture* ToneMappingGL::CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer )
					{
						if ( m_shader == nullptr )
							Init();
						if ( m_shader == nullptr )
						{
							TCMFAILURE("Shader not init");
							return nullptr;
						}

						buffer->BindBuffer();
						glUniform1f( exposureLocation, exposure );

						AbstractRenderer::RenderScreen( texture, m_shader );

						return buffer->GetTexture();
					}

					GrayScaleGL::GrayScaleGL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/grayScale.frag";
					}

					InvertedGL::InvertedGL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/inverted.frag";
					}

					KernelEffectGL::KernelEffectGL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/kernelEffect.frag";
					}

					BlurGL::BlurGL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/blur.frag";
					}

					GaussianBlur5GL::GaussianBlur5GL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/gaussBlur5.frag";
					}

					AbstractTexture* GaussianBlur5GL::CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer )
					{
						if ( m_shader == nullptr )
							Init();
						if ( m_shader == nullptr )
						{
							TCMFAILURE("Shader not init");
							return nullptr;
						}

						bool horizontal = true, first_iteration = true;
						for ( int i = 0; i < nbrPass; i++ )
						{
							m_pingPongBuffers[horizontal]->BindBuffer();

							glUniform1i( 14, horizontal );

							AbstractRenderer::RenderScreen( first_iteration ? texture : m_pingPongBuffers[!horizontal]->GetTexture(), m_shader );

							horizontal = !horizontal;
							if ( first_iteration )
								first_iteration = false;
						}

						buffer->BindBuffer();
						buffer->ClearBuffer();
						AbstractRenderer::RenderScreen( m_pingPongBuffers[!horizontal]->GetTexture() );

						return buffer->GetTexture();
					}

					EdgeGL::EdgeGL()
					{
						fragmentShaderStr = "../Common/TCMRenderer/OpenGLShaders/PostProcess/edge.frag";
					}

					PostProcess* PostProcessManagerGL::CoreGetPostProcess( PostProcessEffect postProcess )
					{
						int index = INT(postProcess);
						if ( m_postProcesses[index] == nullptr )
						{
							PostProcess* p = nullptr;
							switch ( postProcess )
							{
								case PostProcessEffect::BLOOM_EFFECT:
									p = new BloomEffectGL();
									break;
								case PostProcessEffect::GRAYSCALE:
									p = new GrayScaleGL();
									break;
								case PostProcessEffect::INVERTED:
									p = new InvertedGL();
									break;
								case PostProcessEffect::TONE_MAPPING:
									p = new ToneMappingGL();
									break;
								case PostProcessEffect::EDGE:
									p = new EdgeGL();
									break;
								case PostProcessEffect::BLUR:
									p = new BlurGL();
									break;
								case PostProcessEffect::GAUSS_BLUR:
									p = new GaussianBlur5GL();
									break;
								default:
									TCMFAILURE("Post process unknown");
									break;
							}
							m_postProcesses[index] = p;
						}

						return m_postProcesses[index];
					}

					std::string PostProcessManagerGL::CoreGetDefaultVertexShader()
					{
						return "../Common/TCMRenderer/OpenGLShaders/simple.vert";
					}
				}
			}
		}
	}
}
