#include "TCMRenderer/RHI/OpenGL/ShaderOpenGL.h"
#include <string>
#include <fstream>
#include <iostream>

#include "TCMLogger/Logger.h"


namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace OpenGL
			{
				ShaderOpenGL::ShaderOpenGL()
					: m_program( 0 )
					, m_vertexShader( 0 )
					, m_fragmentShader( 0 )
					, m_geometryShader( 0 )
				{
				}

				ShaderOpenGL::~ShaderOpenGL()
				{
					Destroy();
				}

				bool ShaderOpenGL::Create()
				{
					m_program = glCreateProgram();
					glAttachShader( m_program, m_vertexShader );
					glAttachShader( m_program, m_fragmentShader );
					//glAttachShader(m_program, m_geometryShader);

					glLinkProgram( m_program );

					int linkSuccess = 0;
					glGetProgramiv( m_program, GL_LINK_STATUS, &linkSuccess );

					if ( !linkSuccess )
					{
						char errorBuffer[4096];
						int errorLen;
						glGetProgramInfoLog( m_program, 4096, &errorLen, errorBuffer );

						std::string message;
						message.append("Error - Program:\n");
						message.append(errorBuffer);
						TCMDEBUG(message);
						return false;
					}

					{
						glBindAttribLocation( m_program, LOCATION_POSITION, "a_Position" );
						glBindAttribLocation( m_program, LOCATION_NORMAL, "a_Normal" );
						glBindAttribLocation( m_program, LOCATION_TEXTURE, "a_TexCoords" );
						glBindAttribLocation( m_program, LOCATION_TANGENT, "a_Tangent" );

						auto texLocation = glGetUniformLocation( m_program, "AmbientMap" );
						glUniform1i( texLocation, AMBIENT_MAP_LOCATION );
						texLocation = glGetUniformLocation( m_program, "DiffuseMap" );
						glUniform1i( texLocation, DIFFUSE_MAP_LOCATION );
						texLocation = glGetUniformLocation( m_program, "SpecularColorMap" );
						glUniform1i( texLocation, SPECULAR_MAP_LOCATION );
						texLocation = glGetUniformLocation( m_program, "ShininessMap" );
						glUniform1i( texLocation, SHININESS_MAP_LOCATION );
						/*
						texLocation = glGetUniformLocation(programID, "EmissiveMap");
						glUniform1i(texLocation, 4);
						*/
						texLocation = glGetUniformLocation( m_program, "BumpMap" );
						glUniform1i( texLocation, BUMP_MAP_LOCATION );

						texLocation = glGetUniformLocation( m_program, "ShadowMap" );
						glUniform1i( texLocation, SHADOW_MAP_LOCATION );

						auto blockIndex = glGetUniformBlockIndex( m_program, "s_light" );
						glUniformBlockBinding( m_program, blockIndex, BINDING_LIGHTBLOCK );

						blockIndex = glGetUniformBlockIndex( m_program, "MaterialBlock" );
						glUniformBlockBinding( m_program, blockIndex, BINDING_MATERIALBLOCK );

						blockIndex = glGetUniformBlockIndex( m_program, "ViewProjectionMatrix" );
						glUniformBlockBinding( m_program, blockIndex, BINDING_CAMERAMATRIX );
					}

					return true;
				}

				void ShaderOpenGL::Destroy() const
				{
					glDeleteShader( m_vertexShader );
					glDeleteShader( m_geometryShader );
					glDeleteShader( m_fragmentShader );
					glDeleteProgram( m_program );
				}

				bool ShaderOpenGL::Load( const GLuint typeOfShader, const char* path )
				{
					GLuint shaderID = glCreateShader( typeOfShader );
					std::string currentShaderType;

					switch ( typeOfShader )
					{
						case GL_VERTEX_SHADER:
						{
							m_vertexShader = shaderID;
							currentShaderType = "Vertex Shader";
							break;
						}

						case GL_FRAGMENT_SHADER:
						{
							m_fragmentShader = shaderID;
							currentShaderType = "Fragment Shader";
							break;
						}

						case GL_GEOMETRY_SHADER:
						{
							m_geometryShader = shaderID;
							currentShaderType = "Geometry Shader";
							break;
						}
						default:
						{
							break; //ERROR
						}
					}

					std::ifstream fileStream;
					fileStream.open( path, std::ios::binary );
					if ( !fileStream.is_open() )
					{
						std::cerr << "TCMShader::Load() - could not load file [" << path << "]" << std::endl;
						return false;
					}
					auto begin = fileStream.tellg();
					fileStream.seekg( 0, std::ios::end );
					auto file_len = fileStream.tellg() - begin;
					char* buffer = new char[file_len + 1];
					fileStream.seekg( 0, std::ios::beg );
					fileStream.read( buffer, file_len );
					fileStream.close();

					buffer[file_len] = '\0';
					glShaderSource( shaderID, 1, &buffer, nullptr );
					delete[] buffer;

					glCompileShader( shaderID );

					int compileSuccess = 0;
					glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compileSuccess );

					if ( !compileSuccess )
					{
						GLsizei max_size = GL_ERROR_MAX_SIZE;
						char error[GL_ERROR_MAX_SIZE];
						glGetShaderInfoLog( shaderID, GL_ERROR_MAX_SIZE, &max_size, error );
						std::cerr << "Error - " << currentShaderType << ":\n" << error << std::endl;
						return false;
					}
					return true;
				}

				void ShaderOpenGL::Bind() const
				{
					glUseProgram( m_program );
				}

				void ShaderOpenGL::Unbind()
				{
					glUseProgram( 0 );
				}
			}
		}
	}
}
