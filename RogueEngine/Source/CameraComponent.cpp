/* Start Header ************************************************************************/
/*!
\file           CameraComponent.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CameraComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CameraComponent.h"
#include "REMath.h"

namespace Rogue
{
	CameraComponent::CameraComponent(const Vec2& pos) :
		m_position{ pos }
	{}

	void CameraComponent::setPosition(const Vec2& pos)
	{
		m_position = pos;
	}

	void CameraComponent::offSetPosition(const Vec2& pos)
	{
		m_position += pos;
	}

	Vec2 CameraComponent::getPosition() const
	{
		return m_position;
	}

	void CameraComponent::setOffset(const Vec2& offset)
	{
		m_offset = offset;
	}

	Vec2 CameraComponent::getOffset() const
	{
		return m_offset;
	}

	void CameraComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		bool m_isMain = getIsActive();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Camera Active?", &m_isMain);
		ImGui::TextWrapped("There can only be 1 active non-world camera at a time, set others to non-active if you want this to be the main camera.");
		setIsActive(m_isMain);

		ImGui::Text("Camera Offset");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("##OffsetX", &m_offset.x);
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("##OffsetY", &m_offset.y);
	}

	void CameraComponent::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool CameraComponent::getIsActive() const
	{
		return m_isActive;
	}

	std::string CameraComponent::Serialize()
	{
		//Position
		std::ostringstream ss;
		ss << m_position.x << ";" << m_position.y << ";";
		ss << m_offset.x << ";" << m_offset.y << ";";
		ss << m_isActive << ";";
		return ss.str();
	}

	void CameraComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1, s2;		//s2 is used if two are needed
		int counter = 0;		//Needed to take in for multiple values
		int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1 parameter to set

		while (std::getline(ss, s1, ';'))
		{
			//If counter has not cleared all sets yet, read into s2
			if (counter < sets)
			{
				if (std::getline(ss, s2, ';'))
				{
					switch (counter)
					{
					case 0:
						setPosition(Vec2(std::stof(s1), std::stof(s2)));
						break;
					case 1:
						setOffset(Vec2(std::stof(s1), std::stof(s2)));
						break;
					case 2:
						setIsActive(std::stof(s1));
						break;
					default:
						break;
					}
				}
			}
			else
			{
				switch (counter)
				{
				case 0:
					setPosition(Vec2(std::stof(s1), std::stof(s2)));
					break;
				case 1:
					setOffset(Vec2(std::stof(s1), std::stof(s2)));
					break;
				case 2:
					setIsActive(std::stof(s1));
					break;
				default:
					break;
				}
			}
			++counter;
		}
	}
}