#pragma once

#include "OpenGLLibs.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"

#define GL_ERROR_MAX_SIZE	255

#define LOCATION_POSITION	0
#define LOCATION_NORMAL		1
#define LOCATION_TEXTURE	2
#define LOCATION_TANGENT	3

#define LOCATION_MODEL	9
#define LOCATION_TIME	10
#define LOCATION_CAMERAPOSITION	11
#define LOCATION_LIGHTSPACE	12

#define LOCATION_SHADOW_ENABLED	13

#define LOCATION_COLOR 14

#define BINDING_CAMERAMATRIX			15
#define BINDING_MATERIALBLOCK			16
#define BINDING_LIGHTBLOCK				17

#define AMBIENT_MAP_LOCATION 0
#define DIFFUSE_MAP_LOCATION 1
#define SPECULAR_MAP_LOCATION 2
#define SHININESS_MAP_LOCATION 3
#define BUMP_MAP_LOCATION 5

#define SHADOW_MAP_LOCATION 10

#define SKYBOX_PROJECTION 2
#define SKYBOX_VIEW 3

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace OpenGL
			{
				/**
				 * \brief Class containing the data needed for one shader on OpenGL
				 */
				class ShaderOpenGL : public AbstractShader
				{
				public:
					ShaderOpenGL();
					virtual ~ShaderOpenGL();

					/**
					 * \brief Create a new Shader
					 * \return True if Shader well created, false otherwise
					 */
					bool Create();

					/**
					 * \brief Destroys the Shader
					 */
					void Destroy() const;

					/**
					 * \brief Loads a shader from a file
					 * \param typeOfShader _IN_ The type of shader to be created
					 * \param path _IN_ The path of the file to be used as shader
					 * \return If True if well loaded, false otherwise
					 */
					bool Load( const GLuint typeOfShader, const char* path );

					/**
					 * \brief Bind this Shader
					 */
					virtual void Bind() const override;

					/**
					 * \brief Unbind all Shaders
					 */
					static void Unbind();

					/**
					 * \brief Get the program ID of this Shader
					 * \return The program ID
					 */
					inline GLuint Get() const
					{
						return m_program;
					}

				private:
					/**
					 * \brief The Program ID
					 */
					GLuint m_program;

					/**
					 * \brief The Vertex Shader ID
					 */
					GLuint m_vertexShader;

					/**
					 * \brief The Fragment Shader ID
					 */
					GLuint m_fragmentShader;

					/**
					 * \brief The Geometry Shader ID
					 */
					GLuint m_geometryShader;
				};
			}
		}
	}
}
