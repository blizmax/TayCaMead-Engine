#include "TCMRenderer/RHI/OpenGL/RendererOpenGL.h"
#include "TCMLogger/Logger.h"

#include "TCMRenderer/RHI/OpenGL/RenderDataOpenGL.h"
#include "TCMRenderer/Camera/CameraData.h"
#include "TCMRenderer/Gizmo/GizmoMesh.h"

#include "MathLib/Mat3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define BUFFER_OFFSET(i) ((void *)(i))

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace OpenGL
			{
#pragma region Constructor and co.
				RendererOpenGL::RendererOpenGL()
					: m_lastVAO( 0 )
					, m_lastShader( 0 )
					, m_lastMaterial( 0 )
					, m_antiAliasingBuffer( nullptr )
					, m_depthFBO( 0 )
					, m_skyboxVAO( 0 )
					, m_skyboxVBO( 0 )
				{
				}

				RendererOpenGL::~RendererOpenGL()
				{
					glDeleteVertexArrays( 1, &m_gizmoVAO );
					glDeleteBuffers( 1, &m_gizmoVBO );

					glDeleteVertexArrays( 1, &m_skyboxVAO );
					glDeleteBuffers( 1, &m_skyboxVBO );

					delete m_antiAliasingBuffer;
				}
#pragma endregion

#pragma region Overriden functions
				void RendererOpenGL::CoreInitializeRenderer( const int width, const int height )
				{
					TCMINFO("Starting window creation");

					m_width = width;
					m_height = height;

					const GLenum err = glewInit();

					std::string message;

					message.append("Drivers version: ");
					message.append(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
					TCMDEBUG(message);

					message.clear();
					message.append("Constructor: ");
					message.append(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));

					TCMDEBUG(message);

					message.clear();
					message.append("GPU: ");
					message.append(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));

					TCMDEBUG(message);

					TCMDEBUG("Supported extensions: ");

					int numExt;
					glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
					for (int index = 0; index < numExt; ++index) 
					{
						message.clear();
						message.append("extension[");
						message.append(std::to_string(index));
						message.append("] = ");
						message.append(reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, index)));
						TCMDEBUG (message);
					}

					if ( err != GLEW_OK )
						throw std::runtime_error( "RendererOpenGL::CreateRenderWindow() - glutCreateWindow() failed - [" + std::string( reinterpret_cast<const char *>(glewGetErrorString( err )) ) + "]" );

					glEnable( GL_DEPTH_TEST );
					glDepthFunc( GL_LESS );
					glEnable( GL_STENCIL_TEST );
					glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
					glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
					glEnable( GL_CULL_FACE );
					/*if (HDR_ENABLED)
						glEnable(GL_FRAMEBUFFER_SRGB);*/

					m_antiAliasingBuffer = new FrameBufferGL();

					TCMINFO("Finished window creation");
					// TODO: change links to be better
					m_basicShader.Load( GL_VERTEX_SHADER, "../Common/TCMRenderer/OpenGLShaders/base.vert" );
					m_basicShader.Load( GL_FRAGMENT_SHADER, "../Common/TCMRenderer/OpenGLShaders/base.frag" );
					m_basicShader.Create();

					m_screenShader.Load( GL_VERTEX_SHADER, "../Common/TCMRenderer/OpenGLShaders/simple.vert" );
					m_screenShader.Load( GL_FRAGMENT_SHADER, "../Common/TCMRenderer/OpenGLShaders/simple.frag" );
					m_screenShader.Create();

					CoreBindShader( &m_screenShader );
					glUniform1i( glGetUniformLocation( m_screenShader.Get(), "screenTexture" ), 0 );

					m_shadowShader.Load( GL_VERTEX_SHADER, "../Common/TCMRenderer/OpenGLShaders/shadow.vert" );
					m_shadowShader.Load( GL_FRAGMENT_SHADER, "../Common/TCMRenderer/OpenGLShaders/shadow.frag" );
					m_shadowShader.Create();

					CoreBindShader( &m_shadowShader );
					glUniform1i( glGetUniformLocation( m_shadowShader.Get(), "ShadowMap" ), 10 );

					InitBuffers( width, height );
					InitGizmoRendering();
					InitSpriteRenderer();
				}

				void RendererOpenGL::CoreShutdown()
				{
					// Nothing to do for OpenGL
				}

				void RendererOpenGL::CoreClear()
				{
					glBindFramebuffer( GL_FRAMEBUFFER, 0 );
					glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
					glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
					glEnable( GL_DEPTH_TEST );
				}

				void RendererOpenGL::CoreRender( Camera::CameraData* camera, const std::vector<const RenderData*>& queueRenderData,
				                                 const std::vector<LightData*>& influentLights )
				{
					glStencilMask( 0x00 );
					ResizeViewPort( m_shadowMapSizeX, m_shadowMapSizeY );
					glCullFace( GL_FRONT );
					// *************************  S H A D O W   M A P P I N G *****************************//

					for ( LightData* currentLight : influentLights )
					{
						for ( const RenderData* currentObj : queueRenderData )
						{
							if ( currentObj->m_extra->m_castShadows == false )
								continue;
							if ( currentLight->IsDirectionnal() )
								continue;
							if ( currentObj->GetWorldMat().GetTranslation( true ).DistanceTo( currentLight->m_position ) > currentLight->GetRadius() )
								continue;
							currentLight->m_shadowFrameBuffer->BindBuffer();
							glClear( GL_DEPTH_BUFFER_BIT );

							AbstractRenderer::CoreDrawRenderData( camera, currentObj, &m_shadowShader );
						}
					}

					glCullFace( GL_BACK );

					ResizeViewPort( camera->GetTexture()->GetWidth(), camera->GetTexture()->GetHeight() );

					// *************************  G   B U F F E R I N G  *****************************//

					/*for ( size_t i = 0; i < objectCount; ++i )
					{
						// TODO: maybe put a reference&
						std::pair< const RenderData *, TCM::Engine::Math::Mat4 >	onePair = objectList[i];
						CoreDrawRenderData( onePair.first, onePair.second);
					}*/

					// *************************  S C E N E   R E N D E R *****************************//

					camera->GetFramebuffer()->BindBuffer();
					/*glStencilFunc( GL_ALWAYS, 1, 0xFF );
					glStencilMask( 0xFF );*/

					//glColorMask( false, false, false, false );
					for ( const RenderData* data : queueRenderData )
						AbstractRenderer::CoreDrawRenderData( camera, data, &m_basicShader );

					//glColorMask( true, true, true, true );
					glDepthMask( false );
					glStencilMask( 0x00 );

					glEnable( GL_BLEND );
					glBlendFunc( GL_ONE, GL_ONE ); // mode additif
					glDepthFunc( GL_LEQUAL );

					for ( LightData* currentLight : influentLights )
					{
						for ( const RenderData* currentObj : queueRenderData )
						{
							if ( !currentLight->IsDirectionnal() )
								if ( currentObj->GetWorldMat().GetTranslation( true ).DistanceTo( currentLight->m_position ) > currentLight->GetRadius() )
									continue;
							CoreDrawRenderData( camera, currentObj, currentLight );
						}
					}
					glDepthMask( true );
					glDepthFunc( GL_LESS );
					glBlendFunc( GL_ONE, GL_ZERO );
					glDisable( GL_BLEND );

					m_lastVAO = 0;
					m_lastShader = 0;
					m_lastMaterial = 0;
				}

				void RendererOpenGL::CoreDrawRenderData( Camera::CameraData* camera, const RenderData* renderData,
				                                         LightData* light, AbstractShader* forcedShader )
				{
					MeshOpenGL* mesh = dynamic_cast<MeshOpenGL *>(renderData->GetMesh());

					ShaderOpenGL* shader = dynamic_cast<ShaderOpenGL *>(forcedShader == nullptr ? renderData->GetShader() : forcedShader);

					CoreBindShader( shader );

					if ( light )
					{
						LightDataGL* lightGL = dynamic_cast<LightDataGL*>(light);
						glBindBufferBase( GL_UNIFORM_BUFFER, BINDING_LIGHTBLOCK, lightGL->m_idUBO );

						/*if ( renderData->m_extra->m_receiveShadows == true )
						{
							BindOneTexture( dynamic_cast< TextureOpenGL * >( lightGL->m_shadowFrameBuffer->GetTexture() ), SHADOW_MAP_LOCATION );
							glUniform1i( LOCATION_SHADOW_ENABLED, true );
						}
						else*/
						{
							glActiveTexture( GL_TEXTURE10 );
							glBindTexture( GL_TEXTURE_2D, 0 );
							glUniform1i( LOCATION_SHADOW_ENABLED, false );
						}
					}
					else
					{
						glActiveTexture( GL_TEXTURE10 );
						glBindTexture( GL_TEXTURE_2D, 0 );
						glUniform1i( LOCATION_SHADOW_ENABLED, false );
					}

					if ( mesh->m_idVAO != m_lastVAO )
					{
						glBindVertexArray( mesh->m_idVAO );
						m_lastVAO = mesh->m_idVAO;
					}

					Math::Vec3 cameraPosition = camera->GetPosition();
					glUniform3fv( LOCATION_CAMERAPOSITION, 1, reinterpret_cast<float *>(&cameraPosition) );
					Math::Mat4 worldMat = renderData->GetWorldMat();
					glUniformMatrix4fv( LOCATION_MODEL, 1, false, reinterpret_cast<float *>(&worldMat) );

					CameraSpecificDataGL* specificData = dynamic_cast<CameraSpecificDataGL*>(camera->GetSpecificData());
					glBindBufferBase( GL_UNIFORM_BUFFER, BINDING_CAMERAMATRIX, specificData->m_UBO );

					MaterialOpenGL* material = dynamic_cast<MaterialOpenGL *>(renderData->GetMaterial());

					GLuint buffer = material->GetIdUbo();
					if ( material != nullptr && buffer != m_lastMaterial )
					{
						BindOneTexture( dynamic_cast<TextureOpenGL *>(material->GetAmbientMap()), AMBIENT_MAP_LOCATION );
						BindOneTexture( dynamic_cast<TextureOpenGL *>(material->GetDiffuseMap()), DIFFUSE_MAP_LOCATION );
						BindOneTexture( dynamic_cast<TextureOpenGL *>(material->GetSpecularColorMap()), SPECULAR_MAP_LOCATION );
						BindOneTexture( dynamic_cast<TextureOpenGL *>(material->GetShininessMap()), SHININESS_MAP_LOCATION );
						//BindOneTexture(dynamic_cast<TextureOpenGL *>(textureMaps.m_emissiveMap), GL_TEXTURE4);
						BindOneTexture( dynamic_cast<TextureOpenGL *>(material->GetBumpMap()), BUMP_MAP_LOCATION );
						glBindBufferBase( GL_UNIFORM_BUFFER, BINDING_MATERIALBLOCK, buffer );
						m_lastMaterial = buffer;
					}

					glDrawElements( GL_TRIANGLES, static_cast<GLsizei>(renderData->GetIndicesCount()), GL_UNSIGNED_SHORT, BUFFER_OFFSET(renderData->GetIndicesOffset() * sizeof( uint16_t ) ) );
				}

				void RendererOpenGL::CoreDrawSkybox( Camera::CameraData* camera, const AbstractSkybox* gSkybox )
				{
					if ( !gSkybox )
						return;

					ResizeViewPort( camera->GetTexture()->GetWidth(), camera->GetTexture()->GetHeight() );

					glStencilMask( 0x00 );
					const SkyboxOpenGL* skybox = dynamic_cast<const SkyboxOpenGL *>(gSkybox);
					camera->GetFramebuffer()->BindBuffer();

					glDepthMask( GL_FALSE );// Remember to turn depth writing off 

					CoreBindShader( m_skyboxShader );

					// TCMSkyBox cube 
					Math::Mat4 viewMat = Math::Mat4( Math::Mat3( camera->m_viewProjectionMatrix->viewMatrix ) );
					glUniformMatrix4fv( SKYBOX_VIEW, 1, GL_FALSE,
					                               reinterpret_cast<float *>(&viewMat) );

					glUniformMatrix4fv( SKYBOX_PROJECTION, 1, GL_FALSE,
					                                     reinterpret_cast<float *>(&camera->m_viewProjectionMatrix->projectionMatrix) );
					glDepthFunc( GL_LEQUAL );
					if ( m_lastVAO != m_skyboxVAO )
					{
						glBindVertexArray( m_skyboxVAO );
						m_lastVAO = m_skyboxVAO;
					}
					glActiveTexture( GL_TEXTURE0 );
					glBindTexture( GL_TEXTURE_CUBE_MAP, skybox->m_idCubemapTex );
					glDrawArrays( GL_TRIANGLES, 0, 36 );
					glDepthMask( GL_TRUE );
					glDepthFunc( GL_LESS );
				}

				void RendererOpenGL::CoreRenderScreen( const AbstractTexture* tex, AbstractShader* shader )
				{
					const TextureOpenGL* texture = dynamic_cast<const TextureOpenGL*>(tex);
					if ( shader )
					{
						CoreBindShader( shader );
					}
					else
					{
						CoreBindShader( &m_screenShader );
					}
					BindOneTexture( texture, GL_TEXTURE0 );
					static GLuint quadVAO = 0;
					static GLuint quadVBO;
					if ( quadVAO == 0 )
					{
						GLfloat quadVertices[] = {
							// Positions        // Texture Coords
							-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
							-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
							1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
							1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
						};
						// Setup plane VAO
						glGenVertexArrays( 1, &quadVAO );
						glGenBuffers( 1, &quadVBO );
						glBindVertexArray( quadVAO );
						m_lastVAO = quadVAO;
						glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
						glBufferData( GL_ARRAY_BUFFER, sizeof( quadVertices ), &quadVertices, GL_STATIC_DRAW );
						glEnableVertexAttribArray( 0 );
						glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(nullptr) );
						glEnableVertexAttribArray( 1 );
						glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)) );
					}
					if ( quadVAO != m_lastVAO )
					{
						glBindVertexArray( quadVAO );
						m_lastVAO = quadVAO;
					}
					glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
				}

				FrameBuffer* RendererOpenGL::CoreCreateFrameBuffer()
				{
					return new FrameBufferGL();
				}

				CameraSpecificData* RendererOpenGL::CoreCreateCameraSpecificData()
				{
					return new CameraSpecificDataGL();
				}

				void RendererOpenGL::CoreUpdateCameraData( Camera::CameraData* camera )
				{
					if ( !camera->GetSpecificData() )
						return;
					CameraSpecificDataGL* dataGL = dynamic_cast<CameraSpecificDataGL*>(camera->GetSpecificData());
					dataGL->UpdateData( camera );
				}

				FrameBuffer* RendererOpenGL::CoreCreateRendererFrameBuffer( const Math::Vec2& size, FrameBufferType type, const int count )
				{
					int width = static_cast<int>(round( m_width * size.x ));
					int height = static_cast<int>(round( m_height * size.y ));

					return new FrameBufferGL( type, width, height, count );
				}

				void RendererOpenGL::CorePasteBufferOnScreen( const FrameBuffer* frameBuffer, const Math::Vec2& position, const Math::Vec2& size )
				{
					GLint width = frameBuffer->GetTexture()->GetWidth();
					GLint height = frameBuffer->GetTexture()->GetHeight();
					GLint offsetX = static_cast<int>(round( position.x * m_width ));
					GLint offsetY = static_cast<int>(round( position.y * m_height ));

					glBindFramebuffer( GL_READ_FRAMEBUFFER, frameBuffer->GetID() );
					glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
					glBlitFramebuffer( 0, 0, width, height, offsetX, offsetY, offsetX + width, offsetY + height, GL_COLOR_BUFFER_BIT, GL_NEAREST );
				}

#pragma region Factory calls
				AbstractTexture* RendererOpenGL::CoreCreateAbstractTexture()
				{
					TextureOpenGL* tex = new TextureOpenGL();

					return tex;
				}

				AbstractMesh* RendererOpenGL::CoreCreateMesh(
					const uint32_t& vertexNum, const float* vertexBuff,
					const uint32_t& indicesNum, const uint16_t* indicesBuff )
				{
					m_lastVAO = 0;
					return new MeshOpenGL( vertexNum, vertexBuff,
					                       indicesNum, indicesBuff );
				}

				AbstractTexture* RendererOpenGL::CoreCreateTexture( const int type, const int width, const int height, const uint8_t* texData )
				{
					return new TextureOpenGL( type, width, height, texData );
				}

				AbstractMaterial* RendererOpenGL::CoreCreateMaterial()
				{
					m_lastMaterial = 0;
					return new MaterialOpenGL();
				}

				AbstractShader* RendererOpenGL::CoreCreateShader(
					const char* vertexShader, const char* fragmentShader,
					const char* geometryShader, const bool deferred )
				{
					ShaderOpenGL* newShader = new ShaderOpenGL();

					if ( vertexShader != nullptr )
						newShader->Load( GL_VERTEX_SHADER, vertexShader );
					if ( fragmentShader != nullptr )
						newShader->Load( GL_FRAGMENT_SHADER, fragmentShader );
					if ( geometryShader != nullptr )
						newShader->Load( GL_GEOMETRY_SHADER, geometryShader );

					newShader->Create();
					newShader->m_isDeferred = deferred;

					return newShader;
				}

				LightData* RendererOpenGL::CoreCreateLight()
				{
					return new LightDataGL( m_shadowMapSizeX, m_shadowMapSizeY );
				}

				AbstractSkybox* RendererOpenGL::CoreCreateSkybox( std::string skyfaces[6] )
				{
					SkyboxOpenGL* newSkybox = new SkyboxOpenGL;

					newSkybox->m_idCubemapTex = LoadCubemap( skyfaces );

					return newSkybox;
				}

				void RendererOpenGL::CoreResizeViewPort( const int width, const int height )
				{
					glViewport( 0, 0, width, height );
				}
#pragma endregion

#pragma region Private methods
				void RendererOpenGL::InitBuffers( int width, int height )// FBO, TexFBO as paramter - generic
				{
#pragma region Post Process Buffer Init
					m_pingPongBuffers[0] = new FrameBufferGL();
					m_pingPongBuffers[0]->GenTexture( TextureType::Color0, width, height );

					m_pingPongBuffers[1] = new FrameBufferGL();
					m_pingPongBuffers[1]->GenTexture( TextureType::Color0, width, height );
#pragma endregion

					m_antiAliasingBuffer->GenTexture( TextureType::Color0, width, height );
				}

				// * Bind one texture on one GL_TEXTURE#
				void RendererOpenGL::BindOneTexture( const TextureOpenGL* texture, const int textureIndex ) const
				{
					if ( texture != nullptr )
					{
						glActiveTexture( GL_TEXTURE0 + textureIndex );
						glBindTexture( GL_TEXTURE_2D, texture->m_idTex );
					}
				}
#pragma endregion

#pragma region Static private callbacks
				uint32_t RendererOpenGL::LoadCubemap( std::string skyfaces[6] ) const
				{
					GLuint textureID;

					glGenTextures( 1, &textureID );
					int width, height;
					unsigned char* image;

					glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );

					for ( GLuint i = 0; i < 6; i++ )
					{
						image = stbi_load( skyfaces[i].c_str(), &width, &height, nullptr, STBI_rgb );
						if ( !image )
							return 0;
						glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, HDR_ENABLED ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
						stbi_image_free( image );
					}

					glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
					glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
					glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
					glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
					glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
					return textureID;
				}

				void RendererOpenGL::InitGizmoRendering()
				{
					glGenVertexArrays( 1, &m_gizmoVAO );
					glGenBuffers( 1, &m_gizmoVBO );

					glBindVertexArray( m_gizmoVAO );
					glBindBuffer( GL_ARRAY_BUFFER, m_gizmoVBO );

					glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), BUFFER_OFFSET(nullptr) );
					glEnableVertexAttribArray( 0 );
					glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), BUFFER_OFFSET(sizeof(float) * 3) );
					glEnableVertexAttribArray( 1 );
					glBindVertexArray( 0 );

					m_gizmoShader.Load( GL_VERTEX_SHADER, "../Common/TCMRenderer/OpenGLShaders/gizmo.vert" );
					m_gizmoShader.Load( GL_FRAGMENT_SHADER, "../Common/TCMRenderer/OpenGLShaders/gizmo.frag" );
					m_gizmoShader.Create();

					m_lastVAO = 0;
				}

				void RendererOpenGL::CorePostProcess( Camera::CameraData* camera )
				{
					GLint width( camera->GetTexture()->GetWidth() );
					GLint height( camera->GetTexture()->GetHeight() );

					ResizeViewPort( m_width, m_height );
					m_antiAliasingBuffer->ClearBuffer();

					glBindFramebuffer( GL_READ_FRAMEBUFFER, camera->GetFramebuffer()->GetID() );
					glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_antiAliasingBuffer->GetID() );
					glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST );

					GenericPostProcess( m_antiAliasingBuffer, camera->GetPostProcessEffects() );

					glBindFramebuffer( GL_READ_FRAMEBUFFER, m_antiAliasingBuffer->GetID() );
					glBindFramebuffer( GL_DRAW_FRAMEBUFFER, camera->GetFramebuffer()->GetID() );
					glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST );
				}

				void RendererOpenGL::CoreRenderGizmoLines( Camera::CameraData* camera, const std::vector<float>& gizmoLines, const int gizmoLinesCount )
				{
					if ( gizmoLinesCount == 0 )
						return;

					ResizeViewPort( camera->GetTexture()->GetWidth(), camera->GetTexture()->GetHeight() );

					camera->GetFramebuffer()->BindBuffer();
					m_gizmoShader.Bind();

					glBindBuffer( GL_ARRAY_BUFFER, m_gizmoVBO );
					glBufferData( GL_ARRAY_BUFFER, gizmoLines.size() * sizeof(float), gizmoLines.data(), GL_STREAM_DRAW );

					if ( m_lastVAO != m_gizmoVAO )
					{
						glBindVertexArray( m_gizmoVAO );
						m_lastVAO = m_gizmoVAO;
					}
					glUniformMatrix4fv( 10, 1, false, reinterpret_cast<float *>(&camera->m_viewProjectionMatrix->mvpMatrix) );

					glDisable( GL_DEPTH_TEST );
					glDrawArrays( GL_LINES, 0, gizmoLinesCount * 2 );
					glEnable( GL_DEPTH_TEST );
				}

				void RendererOpenGL::CoreRenderGizmoPoints( Camera::CameraData* camera, const std::vector<float>& gizmoPoints, const int gizmoPointsCount )
				{
					if ( gizmoPointsCount == 0 )
						return;

					ResizeViewPort( camera->GetTexture()->GetWidth(), camera->GetTexture()->GetHeight() );

					camera->GetFramebuffer()->BindBuffer();
					m_gizmoShader.Bind();

					glBindBuffer( GL_ARRAY_BUFFER, m_gizmoVBO );
					glBufferData( GL_ARRAY_BUFFER, gizmoPoints.size() * sizeof(float), gizmoPoints.data(), GL_STREAM_DRAW );

					if ( m_lastVAO != m_gizmoVAO )
					{
						glBindVertexArray( m_gizmoVAO );
						m_lastVAO = m_gizmoVAO;
					}

					glUniformMatrix4fv( 10, 1, false, reinterpret_cast<float *>(&camera->m_viewProjectionMatrix->mvpMatrix) );

					glDisable( GL_DEPTH_TEST );
					glDrawArrays( GL_POINTS, 0, gizmoPointsCount );
					glEnable( GL_DEPTH_TEST );
				}

				void RendererOpenGL::CoreRenderGizmoMesh( Camera::CameraData* camera, const Debug::GizmoMesh& gizmoMesh )
				{
					MeshOpenGL* mesh = dynamic_cast<MeshOpenGL *>(gizmoMesh.GetMesh());

					CoreBindShader( &m_basicShader );

					if ( mesh->m_idVAO != m_lastVAO )
					{
						glBindVertexArray( mesh->m_idVAO );
						m_lastVAO = mesh->m_idVAO;
					}

					Math::Vec3 cameraPosition = camera->GetPosition();
					glUniform3fv( LOCATION_CAMERAPOSITION, 1, reinterpret_cast<float *>(&cameraPosition) );
					Math::Mat4 worldMat = gizmoMesh.GetMatrix();
					glUniformMatrix4fv( LOCATION_MODEL, 1, false, reinterpret_cast<float *>(&worldMat) );

					CameraSpecificDataGL* specificData = dynamic_cast<CameraSpecificDataGL*>(camera->GetSpecificData());
					glBindBufferBase( GL_UNIFORM_BUFFER, BINDING_CAMERAMATRIX, specificData->m_UBO );

					if ( gizmoMesh.GetIsWired() )
						glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

					glDrawElements( GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndicesCount()), GL_UNSIGNED_SHORT, 0 );

					if ( gizmoMesh.GetIsWired() )
						glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				}

				void RendererOpenGL::CoreResizeRenderer( const int width, const int height )
				{
					m_pingPongBuffers[0]->ResizeTexture( TextureType::Color0, width, height );
					m_pingPongBuffers[1]->ResizeTexture( TextureType::Color0, width, height );

					m_antiAliasingBuffer->ResizeTexture( TextureType::Color0, width, height );
				}

				void RendererOpenGL::CoreRenderSprite( Camera::CameraData* camera, std::pair<const AbstractTexture*, Math::Mat4> sprite, bool isInCanvas )
				{
					camera->GetFramebuffer()->BindBuffer();
					CoreBindShader( &m_basicShader );
					if ( m_spriteVAO != m_lastVAO )
					{
						glBindVertexArray( m_spriteVAO );
						m_lastVAO = m_spriteVAO;
					}
					BindOneTexture( dynamic_cast<const TextureOpenGL *>(sprite.first), GL_TEXTURE0 );

					glUniformMatrix4fv( LOCATION_MODEL, 1, false, reinterpret_cast<float *>(&sprite.second) );

					CameraSpecificDataGL* specificData = dynamic_cast<CameraSpecificDataGL*>(camera->GetSpecificData());
					glBindBufferBase( GL_UNIFORM_BUFFER, BINDING_CAMERAMATRIX, specificData->m_UBO );

					glDrawArrays( GL_TRIANGLES, 0, 6 );
				}

				void RendererOpenGL::CoreInitSkybox()
				{
					ShaderOpenGL* shader = new ShaderOpenGL();

					shader->Load( GL_VERTEX_SHADER, "../Common/TCMRenderer/OpenGLShaders/skybox.vert" );
					shader->Load( GL_FRAGMENT_SHADER, "../Common/TCMRenderer/OpenGLShaders/skybox.frag" );
					shader->Create();

					glUniform1i( glGetUniformLocation( shader->Get(), "skybox" ), 0 );

					m_skyboxShader = shader;

					float skyBoxVertices[] = {
						// Positions           
						-1.0f, 1.0f, -1.0f,
						-1.0f, -1.0f, -1.0f,
						1.0f, -1.0f, -1.0f,
						1.0f, -1.0f, -1.0f,
						1.0f, 1.0f, -1.0f,
						-1.0f, 1.0f, -1.0f,
						-1.0f, -1.0f, 1.0f,
						-1.0f, -1.0f, -1.0f,
						-1.0f, 1.0f, -1.0f,
						-1.0f, 1.0f, -1.0f,
						-1.0f, 1.0f, 1.0f,
						-1.0f, -1.0f, 1.0f,
						1.0f, -1.0f, -1.0f,
						1.0f, -1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, 1.0f, -1.0f,
						1.0f, -1.0f, -1.0f,
						-1.0f, -1.0f, 1.0f,
						-1.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, -1.0f, 1.0f,
						-1.0f, -1.0f, 1.0f,
						-1.0f, 1.0f, -1.0f,
						1.0f, 1.0f, -1.0f,
						1.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f, 1.0f,
						-1.0f, 1.0f, -1.0f,
						-1.0f, -1.0f, -1.0f,
						-1.0f, -1.0f, 1.0f,
						1.0f, -1.0f, -1.0f,
						1.0f, -1.0f, -1.0f,
						-1.0f, -1.0f, 1.0f,
						1.0f, -1.0f, 1.0f
					};
					// Setup TCMSkyBox VAO 
					glGenVertexArrays( 1, &m_skyboxVAO );
					glGenBuffers( 1, &m_skyboxVBO );

					glBindVertexArray( m_skyboxVAO );
					glBindBuffer( GL_ARRAY_BUFFER, m_skyboxVBO );
					glBufferData( GL_ARRAY_BUFFER, sizeof( skyBoxVertices ), &skyBoxVertices, GL_STATIC_DRAW );

					glEnableVertexAttribArray( 0 );
					glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat), static_cast<GLvoid*>(nullptr) );
					glBindVertexArray( 0 );

					m_lastVAO = 0;
				}

				void RendererOpenGL::InitSpriteRenderer()
				{
					// Configure VAO/VBO
					GLfloat vertices[] = {
						// Pos      // Tex
						0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
						1.0f, 0.0f, 0.0f, 1.0f, 0.0f
					};
					glGenVertexArrays( 1, &m_spriteVAO );
					glGenBuffers( 1, &m_spriteVBO );

					glBindVertexArray( m_spriteVAO );
					glBindBuffer( GL_ARRAY_BUFFER, m_spriteVBO );
					glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

					glEnableVertexAttribArray( LOCATION_POSITION );
					glVertexAttribPointer( LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(nullptr) );
					glEnableVertexAttribArray( LOCATION_TEXTURE );
					glVertexAttribPointer( LOCATION_TEXTURE, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)) );
					glBindBuffer( GL_ARRAY_BUFFER, 0 );
					glBindVertexArray( 0 );
				}
			}
		}
	}
}
