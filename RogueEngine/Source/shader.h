/* Start Header ************************************************************************/
/*!
\file           Shader.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Shader

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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