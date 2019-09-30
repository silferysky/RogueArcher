#ifndef SHADER_H
#define SHADER_H

#include <iostream>

#include <GL/glew.h>

GLuint CompileShader(unsigned int type, const std::string& source);
GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

#endif
