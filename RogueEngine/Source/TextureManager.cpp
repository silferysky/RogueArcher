#define STB_IMAGE_IMPLEMENTATION
#include "TextureManager.h"
#include "stb_image.h"
#include <iostream>

namespace Rogue
{
	TextureManager::~TextureManager()
	{
		for (auto itr = textureMap.begin(); itr != textureMap.end(); ++itr)
		{
			glDeleteTextures(1, &(itr->second.m_texture));
			stbi_image_free(itr->second.m_data);
		}
	}

	std::map<std::string, Texture> TextureManager::getTextureMap() const
	{
		return textureMap;
	}

	Texture TextureManager::loadTexture(const char* texture)
	{
		auto itr = textureMap.find(std::string(texture));
		if (itr != textureMap.end())
			return itr->second;
		else
		{
			Texture newTexture;
			int comp;

			stbi_set_flip_vertically_on_load(true);
			newTexture.m_data = stbi_load // load an image file directly as a new OpenGL texture 
			(
				texture,
				&newTexture.m_width,
				&newTexture.m_height,
				&comp,
				4
			);

			if (newTexture.m_data == NULL)
				RE_INFO("STBI error: ", stbi_failure_reason());

			textureMap.emplace(std::string(texture), newTexture);

			auto itr1 = textureMap.find(texture);
			return itr1->second;
		}
	}
}