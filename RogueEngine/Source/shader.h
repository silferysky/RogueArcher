#pragma once
#include <GL/glew.h>
#include <string>

namespace Rogue
{
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
}