#pragma once
#include <GL/glew.h>
#include <map>
#include "SOIL.h"
#include <iostream>

namespace Rogue
{
	// to properly compare texture paths
	struct str_cmp
	{
		bool operator()(char const* lhs, char const* rhs) const
		{
			return std::strcmp(lhs, rhs) < 0;
		}
	};

	class TextureManager
	{
		std::map<const char*, GLuint, str_cmp> textureMap;
	public:
		TextureManager() = default;
		~TextureManager()
		{
			for (auto itr = textureMap.begin(); itr != textureMap.begin(); ++itr)
				glDeleteTextures(1, &(itr->second));
		}

		std::map<const char*, GLuint, str_cmp> getTextureMap() const
		{
			return textureMap;
		}

		GLuint loadTexture(const char* texture)
		{
			auto itr = textureMap.find(texture);
			if (itr != textureMap.end())
				return itr->second;
			else
			{
				GLuint newTexture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture 
				(
					texture,
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
				);

				if (newTexture == 0)
					std::cout << "SOIL error: " << SOIL_last_result() << std::endl;

				textureMap.emplace(texture, newTexture);

				auto itr1 = textureMap.find(texture);
				return itr1->second;
			}
		}
	};
}