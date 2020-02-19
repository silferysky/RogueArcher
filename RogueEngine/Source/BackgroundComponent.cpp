/* Start Header ************************************************************************/
/*!
\file           BackgroundComponent.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BackgroundComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "BackgroundComponent.h"

namespace Rogue
{
	float BackgroundComponent::GetParallax() const
	{
		return m_parallax;
	}

	void BackgroundComponent::SetParallax(const float& parallax)
	{
		m_parallax = parallax;
	}

	std::string BackgroundComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_parallax << ";";
		return ss.str();
	}

	void BackgroundComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed

		while (std::getline(ss, s1, ';'))
		{
			SetParallax(std::stof(s1));
		}
	}

	void BackgroundComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImGui::DragFloat("Background Parallax", &m_parallax, 0.01f, 0.0f, 10.0f);
		SetParallax(m_parallax);
	}
}