/* Start Header ************************************************************************/
/*!
\file           TextureManager.h
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