#include "PlayerControllerComponent.h"
#include <sstream>

namespace Rogue
{
	void PlayerControllerComponent::SetSlowTime(const float& slowTime)
	{
		m_slowTime = slowTime;
	}

	float PlayerControllerComponent::GetSlowTime() const
	{
		return m_slowTime;
	}

	std::string PlayerControllerComponent::Serialize()
	{
		return std::string(std::to_string(m_isActive));
	}

	void PlayerControllerComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;

		std::getline(ss, s1, ';');
		m_isActive = std::stoi(s1);
	}

}