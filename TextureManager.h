#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
#include "SOIL.h"

class TextureManager
{
	std::map<const char*, GLuint> textureMap;
public:
	TextureManager() = default;
	~TextureManager() = default;

	std::map<const char*, GLuint> getTextureMap() const
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
			GLuint newTexture = SOIL_load_OGL_texture
			(
				texture,
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y
			);

			textureMap.emplace(texture, newTexture);

			return newTexture;
		}
	}
};