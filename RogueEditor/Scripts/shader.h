#ifndef SHADER_H
#define SHADER_H

#include <iostream>

#include "glew.h"
#include <glfw3.h>

unsigned int CompileShader(unsigned int type, const std::string& source);
unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

#endif
