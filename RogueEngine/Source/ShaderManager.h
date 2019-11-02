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

		void Init()
		{
			ShaderMap.emplace("Object Shader", Shader(directory + "vertexShader.txt", directory + "fragmentShader.txt"));
			ShaderMap.emplace("Screen Shader", Shader(directory + "vertexScreenShader.txt", directory + "fragmentScreenShader.txt"));
			ShaderMap.emplace("Debug Shader", Shader(directory + "vertexLineShader.txt", directory + "fragmentLineShader.txt"));
			ShaderMap.emplace("Font Shader", Shader(directory + "vertexFontShader.txt", directory + "fragmentFontShader.txt"));
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