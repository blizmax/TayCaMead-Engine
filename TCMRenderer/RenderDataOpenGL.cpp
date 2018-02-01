#include "TCMRenderer/RHI/OpenGL/RenderDataOpenGL.h"
#include "TCMRenderer/RHI/OpenGL/ShaderOpenGL.h"
#include "TCMRenderer/Camera/CameraManager.h"
#include "MathLib/Mat4.h"
#include "TCMLogger/Logger.h"

#define BUFFER_OFFSET(i) ((void *)(i))

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace OpenGL
			{
#pragma region Mesh OpenGL
				MeshOpenGL::MeshOpenGL(const uint32_t& vertexNum, const float* vertexBuff, const uint32_t& indicesNum, const uint16_t* indicesBuff)
					: s_AbstractMesh(indicesNum)
				{
					glGenVertexArrays(1, &m_idVAO);
					glBindVertexArray(m_idVAO);
					glGenBuffers(1, &m_idVBO);
					glBindBuffer(GL_ARRAY_BUFFER, m_idVBO);
					glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(float), vertexBuff, GL_STATIC_DRAW);
					glGenBuffers(1, &m_idIBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idIBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNum * sizeof(uint16_t), indicesBuff, GL_STATIC_DRAW);
					{
						//void glBindAttribLocation(GLuint program​, GLuint index​, const GLchar *name​);
						GLuint positionLocation = LOCATION_POSITION;
						glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 11 * sizeof(float), BUFFER_OFFSET(nullptr));
						glEnableVertexAttribArray(positionLocation);

						GLuint normalLocation = LOCATION_NORMAL;
						glVertexAttribPointer(normalLocation, 3, GL_FLOAT, false, 11 * sizeof(float), BUFFER_OFFSET(sizeof(float) * 3));
						glEnableVertexAttribArray(normalLocation);

						GLuint texCoordLocation = LOCATION_TEXTURE;
						glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, false, 11 * sizeof(float), BUFFER_OFFSET(sizeof(float) * 6));
						glEnableVertexAttribArray(texCoordLocation);

						GLuint tangentLocation = LOCATION_TANGENT;
						glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, false, 11 * sizeof(float), BUFFER_OFFSET(sizeof(float) * 8));
						glEnableVertexAttribArray(tangentLocation);
					}
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}

				MeshOpenGL::~MeshOpenGL()
				{
					glDeleteVertexArrays(1, &m_idVAO);
					GLuint temp[] = { m_idVBO, m_idIBO };
					glDeleteBuffers(2, temp);
				}
#pragma endregion

#pragma region Texture OpenGL

				TextureOpenGL::TextureOpenGL()
				{
					glGenTextures(1, &m_idTex);
				}

				TextureOpenGL::TextureOpenGL(const int type, const int width, const int height, const uint8_t* texData)
					: s_AbstractTexture(type, width, width)
				{
					glGenTextures(1, &m_idTex);
					glBindTexture(GL_TEXTURE_2D, m_idTex);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					GLint format;
					if (GetType() == 3)
						format = GL_RGB8;
					else
						format = GL_RGBA8;
					glTexImage2D(GL_TEXTURE_2D, 0, format, width, width, 0 // params mem video
						, GL_RGBA, GL_UNSIGNED_BYTE, texData);
					glGenerateMipmap(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				TextureOpenGL::~TextureOpenGL()
				{
					glDeleteTextures(1, &m_idTex);
				}
#pragma endregion

#pragma region Material OpenGL
				MaterialOpenGL::MaterialOpenGL()
				{
					glGenBuffers(1, &m_idUBO);
					glBindBuffer(GL_UNIFORM_BUFFER, m_idUBO);
					glBufferData(GL_UNIFORM_BUFFER, m_size, &_ambientColor, GL_STREAM_DRAW);
					glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_MATERIALBLOCK, m_idUBO);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}

				MaterialOpenGL::~MaterialOpenGL()
				{
					glDeleteBuffers(1, &m_idUBO);
				}

				void MaterialOpenGL::UpdateData()
				{
					glBindBuffer(GL_UNIFORM_BUFFER, m_idUBO);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, &_ambientColor);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}

				GLuint MaterialOpenGL::GetIdUbo() const
				{
					return m_idUBO;
				}

				LightDataGL::LightDataGL(const int width, const int height)
					: s_LightData(width, height)
				{
					glGenBuffers(1, &m_idUBO);
					glBindBuffer(GL_UNIFORM_BUFFER, m_idUBO);
					glBufferData(GL_UNIFORM_BUFFER, m_size, &m_lightSpace, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);

					m_shadowFrameBuffer = new FrameBufferGL();
					m_shadowFrameBuffer->BindBuffer();

					uint32_t depthMap;
					glGenTextures(1, &depthMap);
					glBindTexture(GL_TEXTURE_2D, depthMap);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					const float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
					glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
					TextureOpenGL* depthMapTex = new TextureOpenGL();
					depthMapTex->m_idTex = depthMap;
					m_shadowFrameBuffer->AttachTexture(TextureType::Depth, depthMapTex);
					glDrawBuffer(GL_NONE);
					glReadBuffer(GL_NONE);
					m_shadowFrameBuffer->UnbindBuffer();
				}

				LightDataGL::~LightDataGL()
				{
					glDeleteBuffers(1, &m_idUBO);
				}

				void LightDataGL::UpdateData()
				{
					glBindBuffer(GL_UNIFORM_BUFFER, m_idUBO);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, &m_lightSpace);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}

				SkyboxOpenGL::~SkyboxOpenGL()
				{
					glDeleteTextures(1, &m_idCubemapTex);
				}
#pragma endregion

				bool RenderDataComparerOpenGL::operator()(const RenderData* dataLeft, const RenderData* dataRight) const
				{
					{
						// VAO
						{
#ifdef _DEBUG
							const MeshOpenGL* leftMesh = dynamic_cast<MeshOpenGL *>(dataLeft->GetMesh());
							if (!leftMesh)
								TCMFATALERROR("LeftMesh is nullptr");
							const MeshOpenGL* rightMesh = dynamic_cast<MeshOpenGL *>(dataRight->GetMesh());
							if (!rightMesh)
								TCMFATALERROR("RightMesh is nullptr");
#else
							const MeshOpenGL* leftMesh = static_cast<MeshOpenGL *>(dataLeft->GetMesh());
							const MeshOpenGL* rightMesh = static_cast<MeshOpenGL *>(dataRight->GetMesh());
#endif

							if (leftMesh->m_idVAO != rightMesh->m_idVAO)
								return leftMesh->m_idVAO > rightMesh->m_idVAO;
					}

						// Shader
						{
#ifdef _DEBUG
							const ShaderOpenGL* leftShader = dynamic_cast<ShaderOpenGL *>(dataLeft->GetShader());
							if (!leftShader)
								TCMFATALERROR("LeftShader is nullptr");
							const ShaderOpenGL* rightShader = dynamic_cast<ShaderOpenGL *>(dataRight->GetShader());
							if (!rightShader)
								TCMFATALERROR("RightShader is nullptr");
#else
							const ShaderOpenGL* leftShader = static_cast<ShaderOpenGL *>(dataLeft->GetShader());
							const ShaderOpenGL* rightShader = static_cast<ShaderOpenGL *>(dataRight->GetShader());
#endif
							if (leftShader->Get() != rightShader->Get())
								return leftShader->Get() > rightShader->Get();
				}

						return false;
			};
		}

				FrameBufferGL::FrameBufferGL(FrameBufferType type)
					: FrameBuffer(type)
					, m_optDepthFBO(0)
				{
					glGenFramebuffers(1, &m_FBO);
				}

				FrameBufferGL::FrameBufferGL(FrameBufferType type, const int width, const int height, int count)
					: s_FrameBuffer(type, width, height, count)
					, m_optDepthFBO(0)
					, m_FBO(0)
				{
					glGenFramebuffers(1, &m_FBO);
					glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

					if (type == FrameBufferType::MSAA)
					{
						GLuint textureColorBufferMultiSampled;
						glGenTextures(1, &textureColorBufferMultiSampled);
						glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
						glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, count, GL_RGB, width, height, GL_TRUE);
						glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

						TextureOpenGL* texture = new TextureOpenGL();
						texture->m_idTex = textureColorBufferMultiSampled;
						texture->SetWidth(width);
						texture->SetHeight(height);
						AttachTexture(TextureType::Color0, GL_TEXTURE_2D_MULTISAMPLE, texture);

						glGenRenderbuffers(1, &m_optDepthFBO);
						glBindRenderbuffer(GL_RENDERBUFFER, m_optDepthFBO);
						glRenderbufferStorageMultisample(GL_RENDERBUFFER, count, GL_DEPTH24_STENCIL8, width, height);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_optDepthFBO);
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
					}
					else
					{
						GLuint* textureColorBuffer = new GLuint[count];
						glGenTextures(count, textureColorBuffer);

						GLuint* attachments = new GLuint[count];

						for (int i = 0; i < count; ++i)
						{
							glBindTexture(GL_TEXTURE_2D, textureColorBuffer[i]);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

							TextureOpenGL* texture = new TextureOpenGL();
							texture->m_idTex = textureColorBuffer[i];
							texture->SetWidth(width);
							texture->SetHeight(height);
							AttachTexture(TextureType::Color0 + i, GL_TEXTURE_2D, texture);

							attachments[i] = GL_COLOR_ATTACHMENT0 + i;
						}

						glDrawBuffers(count, attachments);
						delete[] textureColorBuffer;
						delete[] attachments;

						glGenRenderbuffers(1, &m_optDepthFBO);
						glBindRenderbuffer(GL_RENDERBUFFER, m_optDepthFBO);
						glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_optDepthFBO);
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
					}
				}

				FrameBufferGL::~FrameBufferGL()
				{
					glDeleteFramebuffers(1, &m_FBO);

					std::vector<uint32_t> FBOs;
					FBOs.resize(mTextures.size());

					for (int i = 0; i < mTextures.size(); i++)
					{
						TextureOpenGL* textureGL = dynamic_cast<TextureOpenGL *>(mTextures[i]);
						FBOs[i] = textureGL->m_idTex;
					}

					glDeleteTextures(static_cast<GLsizei>(FBOs.size()), FBOs.data());
				}

				void FrameBufferGL::BindBuffer() const
				{
					glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
				}

				void FrameBufferGL::UnbindBuffer()
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				void FrameBufferGL::ClearBuffer(Math::ColorRGB color)
				{
					BindBuffer();
					glClearColor(color.r, color.g, color.b, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				}

				void FrameBufferGL::CoreResize(const int width, const int height) const
				{
					BindBuffer();

					if (GetType() == FrameBufferType::MSAA)
					{
						TextureOpenGL* texture = dynamic_cast<TextureOpenGL*>(GetTextureOfType(TextureType::Color0));

						glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->m_idTex);
						glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, GetCount(), GL_RGB, width, height, GL_TRUE);
						glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
						texture->SetWidth(width);
						texture->SetHeight(height);

						LinkTexture(TextureType::Color0, GL_TEXTURE_2D_MULTISAMPLE, texture);

						glBindRenderbuffer(GL_RENDERBUFFER, m_optDepthFBO);
						glRenderbufferStorageMultisample(GL_RENDERBUFFER, GetCount(), GL_DEPTH24_STENCIL8, width, height);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_optDepthFBO);
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
					}
					else
					{
						int count = GetCount();
						for (int i = 0; i < count; ++i)
						{
							TextureType t = TextureType::Color0 + i;
							TextureOpenGL* texture = dynamic_cast<TextureOpenGL*>(GetTextureOfType(t));

							glBindTexture(GL_TEXTURE_2D, texture->m_idTex);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
							glBindTexture(GL_TEXTURE_2D, 0);
							texture->SetWidth(width);
							texture->SetHeight(height);

							LinkTexture(t, GL_TEXTURE_2D, texture);
						}

						glBindRenderbuffer(GL_RENDERBUFFER, m_optDepthFBO);
						glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_optDepthFBO);
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
					}
				}

				AbstractTexture* FrameBufferGL::GenTexture(TextureType type, int width, int height)
				{
					AbstractTexture* tex = GetTextureOfType(type);
					if (tex != nullptr)
						return tex;

					uint32_t texFBO;

					glGenTextures(1, &texFBO);

					glBindTexture(GL_TEXTURE_2D, texFBO);

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					TextureOpenGL* texGL = new TextureOpenGL();
					texGL->m_idTex = texFBO;
					texGL->SetWidth(width);
					texGL->SetHeight(height);

					AttachTexture(type, texGL);

					return texGL;
				}

				AbstractTexture* FrameBufferGL::ResizeTexture(TextureType type, int width, int height)
				{
					TextureOpenGL* tex = dynamic_cast<TextureOpenGL*>(GetTextureOfType(type));
					if (tex == nullptr)
						return nullptr;

					tex->SetWidth(width);
					tex->SetHeight(height);

					glBindTexture(GL_TEXTURE_2D, tex->m_idTex);

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
					glFramebufferTexture2D(GL_FRAMEBUFFER, ConvertType(type), GL_TEXTURE_2D, tex->m_idTex, 0);

					return tex;
				}

				void FrameBufferGL::AttachTexture(TextureType type, AbstractTexture* texture)
				{
					LinkTexture(type, GL_TEXTURE_2D, texture);

					FrameBuffer::AttachTexture(type, texture);
				}

				void FrameBufferGL::AttachTexture(TextureType type, int typeTexture, AbstractTexture* texture)
				{
					LinkTexture(type, typeTexture, texture);

					FrameBuffer::AttachTexture(type, texture);
				}

				void FrameBufferGL::LinkTexture(TextureType type, int typeTexture, AbstractTexture* texture) const
				{
					TextureOpenGL* textureGL = dynamic_cast<TextureOpenGL *>(texture);

					glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
					glFramebufferTexture2D(GL_FRAMEBUFFER, ConvertType(type), typeTexture, textureGL->m_idTex, 0);
				}

				int FrameBufferGL::GetID() const
				{
					return m_FBO;
				}

				int FrameBufferGL::ConvertType(TextureType t) const
				{
					switch (t)
					{
					case TextureType::Color0:
						return GL_COLOR_ATTACHMENT0;
					case TextureType::Color1:
						return GL_COLOR_ATTACHMENT1;
					case TextureType::Color2:
						return GL_COLOR_ATTACHMENT2;
					case TextureType::Color3:
						return GL_COLOR_ATTACHMENT3;
					case TextureType::Color4:
						return GL_COLOR_ATTACHMENT4;
					case TextureType::Color5:
						return GL_COLOR_ATTACHMENT5;
					case TextureType::Color6:
						return GL_COLOR_ATTACHMENT6;
					case TextureType::Color7:
						return GL_COLOR_ATTACHMENT7;
					case TextureType::Color8:
						return GL_COLOR_ATTACHMENT8;
					case TextureType::Color9:
						return GL_COLOR_ATTACHMENT9;
					case TextureType::Color10:
						return GL_COLOR_ATTACHMENT10;
					case TextureType::Depth:
						return GL_DEPTH_ATTACHMENT;
					case TextureType::Stencil:
						return GL_STENCIL_ATTACHMENT;
					case TextureType::Depth_Stencil:
						return GL_DEPTH_STENCIL_ATTACHMENT;
					case TextureType::Count:
						return 0;
					default:
						return 0;
					}
				}

				CameraSpecificDataGL::CameraSpecificDataGL()
				{
					glGenBuffers(1, &m_UBO);
					glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
					glBufferData(GL_UNIFORM_BUFFER, sizeof(Math::Mat4) * 2, nullptr, GL_STREAM_DRAW);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}

				CameraSpecificDataGL::~CameraSpecificDataGL()
				{
					glDeleteBuffers(1, &m_UBO);
				}

				void CameraSpecificDataGL::UpdateData(Camera::CameraData* data)
				{
					glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Math::Mat4) * 2, &data->m_viewProjectionMatrix->viewMatrix);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}
	}
}
	}
}
