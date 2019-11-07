#pragma once

#include <map>
#include <GL/glew.h>
#include "Logger.h"

namespace Rogue
{
	// to properly compare texture paths
	/*struct str_cmp
	{
		bool operator()(char const* lhs, char const* rhs) const
		{
			return std::strcmp(lhs, rhs) < 0;
		}
	};*/

	struct Texture
	{
		GLuint m_texture;
		int m_width;
		int m_height;
		unsigned char* m_data;
	};

	class TextureManager
	{
		std::map<std::string, Texture> textureMap;
	public:
		static TextureManager& instance()
		{
			static TextureManager instance;
			return instance;
		}

		TextureManager() = default;
		~TextureManager();

		std::map<std::string, Texture>& getTextureMap();
		
		Texture loadTexture(const char* texture);
	};
}