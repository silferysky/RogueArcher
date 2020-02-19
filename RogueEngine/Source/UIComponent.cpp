/* Start Header ************************************************************************/
/*!
\file           UIComponent.cpp
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
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImGui::PushItemWidth(75);
		ImGui::Checkbox("UI Active?", &m_isActive);
		setIsActive(m_isActive);
	}

	std::string UIComponent::Serialize()
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