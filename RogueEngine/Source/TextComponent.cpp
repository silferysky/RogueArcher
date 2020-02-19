/* Start Header ************************************************************************/
/*!
\file           TextComponent.cpp
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
#include "Precompiled.h"
#include "TextComponent.h"

namespace Rogue
{
	void TextComponent::SetColour(const glm::vec4& colour)
	{
		m_colour = colour;
	}

	glm::vec4& TextComponent::GetColour()
	{
		return m_colour;
	}

	void TextComponent::SetWords(std::string_view words)
	{
		m_words = words;
	}

	std::string_view TextComponent::GetWords()
	{
		return m_words;
	}

	void TextComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		static char m_newwords[128];
		ImGui::PushItemWidth(75);
		ImGui::TextWrapped("Current Text : ");
		ImGui::TextWrapped("%s", m_words.c_str());
		ImGui::TextDisabled("New Text");
		ImGui::SameLine();
		ImGui::PushItemWidth(250);
		ImGui::InputText("                    ", m_newwords, 128);
		if (ImGui::Button("Set new text"))
		{
			SetWords(m_newwords);
			memset(m_newwords, 0, 128);
		}

		ImGui::PushItemWidth(250);
		ImGui::ColorEdit4("Color", (float*)& m_colour);
		SetColour(m_colour);
	}

	std::string TextComponent::Serialize()
	{
		//Position
		std::ostringstream ss;
		ss << m_colour.x << ";" << m_colour.y << ";" << m_colour.z << ";" << m_colour.w << ";";
		ss << m_words << ";";
		return ss.str();
	}

	void TextComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1, s2, s3, s4;		//s2 is used if two are needed
		int counter = 0;		//Needed to take in for multiple values
		int sets = 1;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1 parameter to set

		while (std::getline(ss, s1, ';'))
		{
			//If counter has not cleared all sets yet, read into s2
			if (counter < sets)
			{
				std::getline(ss, s2, ';');
				std::getline(ss, s3, ';');
				std::getline(ss, s4, ';');
			}

			switch (counter)
			{
			case 0:
				SetColour(glm::vec4(std::stof(s1), std::stof(s2), std::stof(s3), std::stof(s4)));
				break;
			case 1:
				SetWords(s1);
				break;
			default:
				break;
			}

			++counter;
		}
	}
};