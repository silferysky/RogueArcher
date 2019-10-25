#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>

namespace Rogue
{
	class SpriteComponent
		: public BaseComponent
	{
		std::string m_texturePath;
		GLuint m_texture;
	public:
		SpriteComponent();
		~SpriteComponent() = default;

		void setTexture(const char* texture);
		GLuint getTexture() const;

		void setTexturePath(std::string texturePath);
		std::string getTexturePath() const;

		//Serialize
		virtual std::string Serialize();
		virtual void Deserialize(std::string toDeserialize);

		//void operator=(SpriteComponent sprite);
	};
}