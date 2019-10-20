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
	public:
		ShaderManager() = default;
		~ShaderManager() = default;

		void Init()
		{
			ShaderMap.emplace("Object Shader", Shader("vertexShader.txt", "fragmentShader.txt"));
			ShaderMap.emplace("Debug Shader", Shader("vertexLineShader.txt", "fragmentLineShader.txt"));
			ShaderMap.emplace("Font Shader", Shader("vertexFontShader.txt", "fragmentFontShader.txt"));
		}

		std::map<std::string, Shader> getShaderMap() const
		{
			return ShaderMap;
		}

		Shader loadShader(std::string shader)
		{
			auto itr = ShaderMap.find(shader);
			if (itr != ShaderMap.end())
				return itr->second;
			/* else
			{
				std::cout << "Error: Shader " << shader << " not found." << std::endl;
				return nullptr;
			} */
		}
	};
}
