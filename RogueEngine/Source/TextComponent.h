#pragma once
#include "BaseComponent.h"
#include <glm.hpp>
#include <string>

namespace Rogue
{ 
	class TextComponent
		: public BaseComponent
	{
		glm::vec4 m_colour;
		std::string m_words;
		float m_scale;
	public:
		void SetColour(const glm::vec4& colour);
		glm::vec4& GetColour();

		void SetWords(const std::string& words);
		std::string& GetWords();

		void SetScale(const float& scale);
		float& GetScale();

		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
};