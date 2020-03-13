/* Start Header ************************************************************************/
/*!
\file           PlayerControllerComponent.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PlayerControllerComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "PlayerControllerComponent.h"


namespace Rogue
{
	PlayerControllerComponent::PlayerControllerComponent() :
		m_moveSpeed{ 5000.0f }, m_slowTime{ 0.1f }, m_isActive{ true }, m_moveState{ MoveState::e_stop }, m_grounded{ false }, m_jumpTimer{ 0.1f }, m_moveForceMultiplier{10.0f},
		m_jumpSpeed{ 15000.0f }
	{}

	void PlayerControllerComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Slow-Mo Time", &m_slowTime, 0.01f, 0.0f, 1.0f);
		SetSlowTime(m_slowTime);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Movement Speed", &m_moveSpeed, 1.0f, 1.0f, 1000000.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Movement Force Multiplier", &m_moveForceMultiplier, 1.0f, 1.0f, 1000000.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Jump Speed", &m_jumpSpeed);
	}

	void PlayerControllerComponent::SetSlowTime(float slowTime)
	{
		m_slowTime = slowTime;
	}

	void PlayerControllerComponent::SetMoveSpeed(float speed)
	{
		m_moveSpeed = speed;
	}

	void PlayerControllerComponent::SetMoveForceMultiplier(float multiplier)
	{
		m_moveForceMultiplier = multiplier;
	}

	void PlayerControllerComponent::SetMoveState(MoveState state)
	{
		m_moveState = state;
	}

	void PlayerControllerComponent::SetJumpSpeed(float speed)
	{
		m_jumpSpeed = speed;
	}

	float PlayerControllerComponent::GetSlowTime() const
	{
		return m_slowTime;
	}

	float PlayerControllerComponent::GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	float PlayerControllerComponent::GetMoveForceMultiplier() const
	{
		return m_moveForceMultiplier;
	}

	float PlayerControllerComponent::GetJumpSpeed() const
	{
		return m_jumpSpeed;
	}

	MoveState PlayerControllerComponent::GetMoveState() const
	{
		return m_moveState;
	}

	void PlayerControllerComponent::SetIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool PlayerControllerComponent::GetIsActive() const
	{
		return m_isActive;
	}

	std::string PlayerControllerComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		ss << m_slowTime << ";";
		ss << m_moveSpeed << ";";
		ss << m_moveForceMultiplier << ";";
		ss << m_jumpSpeed << ";";
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
			m_moveSpeed = std::stof(s1);

		if (std::getline(ss, s1, ';'))
			m_moveForceMultiplier = std::stof(s1);

		if (std::getline(ss, s1, ';'))
			m_jumpSpeed = std::stof(s1);
	}

}