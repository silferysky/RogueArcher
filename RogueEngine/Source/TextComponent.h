#pragma once
#include <glm.hpp>
#include <string>
#include "BaseComponent.h"

namespace Rogue
{ 
	class TextComponent: public BaseComponent
	{
		glm::vec4 m_colour;
		std::string m_words;
	public:
		void SetColour(const glm::vec4& colour);
		glm::vec4& GetColour();

		void SetWords(std::string_view words);
		std::string_view GetWords();

		void DisplayOnInspector();

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
};