#include "..\Common\TCMRenderer\TCMShaderProgram.h"
#include "GL\glew.h"
#include "Utility\Utility.h"
#include <string>
#include <fstream>
#include <iostream>

TCMShaderProgram::TCMShaderProgram()
	: m_program(0)
	, m_vertexShader(0)
	, m_fragmentShader(0)
	, m_geometryShader(0)
{
}

TCMShaderProgram::~TCMShaderProgram()
{
}

bool	TCMShaderProgram::Create()
{
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	//glAttachShader(m_program, m_geometryShader);

	glLinkProgram(m_program);

	int linkSuccess = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkSuccess);

	if (!linkSuccess)
	{
		char errorBuffer[4096];
		int errorLen;
		glGetProgramInfoLog(m_program, 4096, &errorLen, errorBuffer);
		std::cerr << "Error - Program:\n" << errorBuffer << std::endl;
		return false;
	}
	return true;
}

void	TCMShaderProgram::Destroy()
{
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_geometryShader);
	glDeleteShader(m_fragmentShader);
	glDeleteProgram(m_program);
}

bool	TCMShaderProgram::Load(const GLuint typeOfShader, const char* path)
{
	GLuint shaderID = glCreateShader(typeOfShader);
	std::string		currentShaderType;

	switch (typeOfShader)
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

			break; //BADTYPE
		}
	}

	std::ifstream fileStream;
	fileStream.open(path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cerr << "TCMShader::Load() - could not load file [" << path << "]" << std::endl;
		return false;
	}
	auto begin = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	auto file_len = fileStream.tellg() - begin;
	char* buffer = new char[file_len + 1];
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(buffer, file_len);
	fileStream.close();

	buffer[file_len] = '\0';
	glShaderSource(shaderID, 1, &buffer, nullptr);
	delete buffer;

	glCompileShader(shaderID);

	int compileSuccess = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileSuccess);

	if (!compileSuccess)
	{
		GLsizei max_size = GL_ERROR_MAX_SIZE;
		char error[GL_ERROR_MAX_SIZE];
		glGetShaderInfoLog(shaderID, GL_ERROR_MAX_SIZE, &max_size, error);
		std::cerr << "Error - " << currentShaderType << ":\n" << error << std::endl;
		return false;
	}	
	return true;
}

void TCMShaderProgram::Bind() const
{
	glUseProgram(m_program);
}

void TCMShaderProgram::Unbind() const
{
	glUseProgram(0);
}