/* Start Header ************************************************************************/
/*!
\file           SpriteComponent.h
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
		float m_texCoordMinX = 0.0f;
		float m_texCoordMaxX = 1.0f;
		float m_texCoordMinY = 0.0f;
		float m_texCoordMaxY = 1.0f;

		Vec2 m_texCoordScale = { 1.0f, 1.0f };
		Vec2 m_texCoordOffset = { 0.0f, 0.0f };

		glm::vec4 m_filter = { 1.0f, 1.0f, 1.0f, 1.0f };
	public:
		SpriteComponent() = default;
		~SpriteComponent() = default;

		void setTexture(const char* texture);
		Texture& getTexture();

		void setTexCoordMinX(const float& TexCoordMin);
		float getTexCoordMinX() const;

		void setTexCoordMaxX(const float& texCoordMax);
		float getTexCoordMaxX() const;
		
		void setTexCoordMinY(const float& TexCoordMinY);
		float getTexCoordMinY() const;

		void setTexCoordMaxY(const float& texCoordMaxY);
		float getTexCoordMaxY() const;

		void setTexCoordScaleX(const float& texCoordScaleX);
		void setTexCoordScaleY(const float& texCoordScaleY);
		Vec2 getTexCoordScale() const;

		void setTexCoordOffsetX(const float& texCoordOffsetX);
		void setTexCoordOffsetY(const float& texCoordOffsetY);
		Vec2 getTexCoordOffset() const;

		void setTexturePath(std::string_view texturePath);
		std::string_view getTexturePath() const;

		glm::vec4& getFilter();
		void setFilter(const glm::vec4& filter);

		void DisplayOnInspector();

		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view toDeserialize) override;

		//void operator=(SpriteComponent sprite);
	};
}