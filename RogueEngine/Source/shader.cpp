#include "Shader.h"
#include <iostream>
#include "BasicIO.h"


Shader::Shader(const std::string& vShader, const std::string& fShader)
{
	std::string vertexShader = BasicIO::ReadFile(vShader);
	std::string fragmentShader = BasicIO::ReadFile(fShader);

	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	/* Combine the shaders into the program */
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	/* Delete intermediates */
	glDetachShader(program, vs);
	glDetachShader(program, fs);

	m_shader = program;
}

GLuint Shader::CompileShader(unsigned int type, const std::string& source) const
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	/* Error checking */
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		std::cout << "Error: Failed to compile shader:" << source << std::endl;
		glDeleteShader(id);

		return 0;
	}

	return id;
}

GLuint Shader::GetShader() const
{
	return m_shader;
}