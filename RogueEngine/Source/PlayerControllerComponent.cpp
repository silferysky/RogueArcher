#include "Precompiled.h"
#include "PlayerControllerComponent.h"


namespace Rogue
{
	void PlayerControllerComponent::DisplayOnInspector()
	{
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Time Scale", &m_slowTime, 0.01f, 0.0f, 1.0f);
		SetSlowTime(m_slowTime);

		ImGui::Text("Movement Speed");
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("X", &m_moveSpeed.x, 1.0f, 1.0f, 99999.0f);
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Y", &m_moveSpeed.y, 1.0f, 1.0f, 99999.0f);
	}

	void PlayerControllerComponent::SetSlowTime(float slowTime)
	{
		m_slowTime = slowTime;
	}

	void PlayerControllerComponent::SetMoveSpeed(const Vec2& speed)
	{
		m_moveSpeed = speed;
	}

	void PlayerControllerComponent::SetMoveSpeedX(float x)
	{
		m_moveSpeed.x = x;
	}

	void PlayerControllerComponent::SetMoveSpeedY(float y)
	{
		m_moveSpeed.y = y;
	}

	float PlayerControllerComponent::GetSlowTime() const
	{
		return m_slowTime;
	}

	Vec2 PlayerControllerComponent::GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	std::string PlayerControllerComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		ss << m_slowTime << ";";
		ss << m_moveSpeed.x << ";";
		ss << m_moveSpeed.y << ";";
		return ss.str();
	}

	void PlayerControllerComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;

		if(std::getline(ss, s1, ';'))
			m_isActive = std::stoi(s1);
		
		if(std::getline(ss, s1, ';'))
			m_slowTime = std::stof(s1);

		if(std::getline(ss, s1, ';'))
			m_moveSpeed.x = std::stof(s1);

		if(std::getline(ss, s1, ';'))
			m_moveSpeed.y = std::stof(s1);
	}

}