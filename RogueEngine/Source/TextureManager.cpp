/* Start Header ************************************************************************/
/*!
\file           TextureManager.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for SpriteComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#define STB_IMAGE_IMPLEMENTATION
#include "TextureManager.h"
#include "stb_image.h"

namespace Rogue
{
	TextureManager::~TextureManager()
	{
		for (auto itr = textureMap.begin(); itr != textureMap.end(); ++itr)
		{
			glDeleteTextures(1, &(itr->second.m_texture));
			//stbi_image_free(itr->second.m_data);
		}
	}

	std::map<std::string, Texture>& TextureManager::getTextureMap()
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

			glGenTextures(1, &newTexture.m_texture);

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
			{
				RE_INFO("STBI error: ", stbi_failure_reason());
			}
			glBindTexture(GL_TEXTURE_2D, newTexture.m_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newTexture.m_width, newTexture.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newTexture.m_data);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			 
			stbi_image_free(newTexture.m_data);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);

			textureMap.emplace(std::string_view(texture), newTexture);

			auto itr1 = textureMap.find(texture);
			return itr1->second;
		}
	}
}