#pragma once

#include "GL\glew.h"
#include <string>

#define GL_ERROR_MAX_SIZE	255

class TCMShaderProgram
{
public:
	TCMShaderProgram();
	~TCMShaderProgram();

	bool	Create();
	void	Destroy();
	bool	Load(const GLuint typeOfShader, const char* path);
	void	Bind() const;
	void	Unbind() const;

	inline GLuint	Get() const
	{
		return m_program;
	}

private:
	GLuint m_program;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_geometryShader;
};

