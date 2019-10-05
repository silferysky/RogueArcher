#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <GL/glew.h>
#include "BasicIO.h"

class Shader
{
	GLuint m_shader;
public:
	Shader() = default;
	~Shader() = default;

	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	GLuint CompileShader(unsigned int type, const std::string& source) const;

	GLuint GetShader() const;
};

#endif
