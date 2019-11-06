#include "TextComponent.h"
#include <sstream>

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

	void TextComponent::SetWords(const std::string& words)
	{
		m_words = words;
	}

	std::string& TextComponent::GetWords()
	{
		return m_words;
	}

	void TextComponent::SetScale(const float& scale)
	{
		m_scale = scale;
	}

	float& TextComponent::GetScale()
	{
		return m_scale;
	}

	std::string TextComponent::Serialize()
	{
		//Position
		std::ostringstream ss;
		ss << m_colour.x << ";" << m_colour.y << ";" << m_colour.z << ";" << m_colour.w << ";";
		ss << m_words << ";";
		ss << m_scale << ";";
		return ss.str();
	}

	void TextComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
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
			case 2:
				SetScale(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
};