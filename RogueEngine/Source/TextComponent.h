/* Start Header ************************************************************************/
/*!
\file           TextComponent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for FontSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <glm.hpp>
#include <string>
#include "BaseComponent.h"

namespace Rogue
{ 
	class TextComponent: public BaseComponent
	{
		glm::vec4 m_colour{};
		std::string m_words{};
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