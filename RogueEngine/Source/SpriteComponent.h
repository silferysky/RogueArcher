#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>
#include "TextureManager.h"
#include <glm.hpp>

namespace Rogue
{
	class SpriteComponent
		: public BaseComponent
	{
		std::string m_texturePath;
		Texture m_texture;
		int m_drawPriority = 0;
		float m_texCoordMin = 0.0f;
		float m_texCoordMax = 1.0f;

		glm::vec4 m_filter = { 1.0f, 1.0f, 1.0f, 1.0f };
	public:
		SpriteComponent() = default;
		~SpriteComponent() = default;

		void setTexture(const char* texture);
		Texture& getTexture();

		void setDrawPriority(const int& drawPriority);
		int getDrawPriority() const;

		void setTexCoordMin(const float& TexCoordMin);
		float getTexCoordMin() const;

		void setTexCoordMax(const float& texCoordMax);
		float getTexCoordMax() const;
		
		void setTexturePath(std::string texturePath);
		std::string getTexturePath() const;

		glm::vec4& getFilter();
		void setFilter(const glm::vec4& filter);

		//Serialize
		virtual std::string Serialize();
		virtual void Deserialize(std::string toDeserialize);

		//void operator=(SpriteComponent sprite);
	};
}