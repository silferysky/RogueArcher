#pragma once
#include "UIComponent.h"
#include <sstream>

namespace Rogue
{
	bool UIComponent::getIsActive() const
	{
		return m_isActive;
	}

	void UIComponent::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	std::string UIComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		return ss.str();
	}

	void UIComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;		//s2 is used if two are needed

		while (std::getline(ss, s1, ';'))
		{
			setIsActive(std::stof(s1));
		}
	}
}