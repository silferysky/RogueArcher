#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>
#include "TextureManager.h"

namespace Rogue
{
	class SpriteComponent
		: public BaseComponent
	{
		std::string m_texturePath;
		Texture m_texture;
	public:
		SpriteComponent() = default;
		~SpriteComponent() = default;

		void setTexture(const char* texture);
		Texture getTexture() const;

		void setTexturePath(std::string texturePath);
		std::string getTexturePath() const;

		//Serialize
		virtual std::string Serialize();
		virtual void Deserialize(std::string toDeserialize);

		//void operator=(SpriteComponent sprite);
	};
}