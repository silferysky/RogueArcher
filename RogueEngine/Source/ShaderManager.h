/* Start Header ************************************************************************/
/*!
\file           ShaderManager.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ShaderManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <map>
#include "Shader.h"

namespace Rogue
{
	class ShaderManager
	{
		std::map<std::string, Shader> ShaderMap;
		std::string directory = "Resources/Shaders/";
	public:
		ShaderManager() = default;
		~ShaderManager() = default;

		static ShaderManager& instance()
		{
			static ShaderManager instance;
			return instance;
		}

		void Init()
		{
			ShaderMap.emplace("Object Shader", Shader(directory + "vertexShader.txt", directory + "fragmentShader.txt"));
			ShaderMap.emplace("Foreground Shader", Shader(directory + "vertexForegroundShader.txt", directory + "fragmentShader.txt"));
			ShaderMap.emplace("Screen Shader", Shader(directory + "vertexScreenShader.txt", directory + "fragmentScreenShader.txt"));
			ShaderMap.emplace("Debug Shader", Shader(directory + "vertexLineShader.txt", directory + "fragmentLineShader.txt"));
			ShaderMap.emplace("Font Shader", Shader(directory + "vertexFontShader.txt", directory + "fragmentFontShader.txt"));
			ShaderMap.emplace("Lighting Shader", Shader(directory + "vertexLightShader.txt", directory + "fragmentLightShader.txt"));
		}

		const std::map<std::string, Shader>& getShaderMap() const
		{
			return ShaderMap;
		}

		Shader loadShader(const std::string& shader)
		{
			auto itr = ShaderMap.find(shader);


			std::stringstream out;
			out << "Shader " << shader << " not found!";
			RE_ASSERT(itr != ShaderMap.end(), out.str().c_str());

			return itr->second;
		}
	};
}