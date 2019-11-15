#pragma once
#include "Precompiled.h"
#include "UIComponent.h"

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

	void UIComponent::DisplayOnInspector()
	{
		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Active?", &m_isActive);
	}

	std::string_view UIComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		return ss.str();
	}

	void UIComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed

		while (std::getline(ss, s1, ';'))
		{
			setIsActive(std::stof(s1));
		}
	}
}