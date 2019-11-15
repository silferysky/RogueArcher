#include "Precompiled.h"
#include "PlayerControllerComponent.h"


namespace Rogue
{
	void PlayerControllerComponent::DisplayOnInspector()
	{
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Time Scale", &m_slowTime, 0.01f, 0.0f, 1.0f);
		SetSlowTime(m_slowTime);
	}

	void PlayerControllerComponent::SetSlowTime(const float& slowTime)
	{
		m_slowTime = slowTime;
	}

	float PlayerControllerComponent::GetSlowTime() const
	{
		return m_slowTime;
	}

	std::string_view PlayerControllerComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		ss << m_slowTime << ";";
		return ss.str();
	}

	void PlayerControllerComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;

		std::getline(ss, s1, ';');
		m_isActive = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_slowTime = std::stof(s1);
	}

}