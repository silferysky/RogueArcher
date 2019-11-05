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
		std::ostringstream ss;
		ss << m_isActive << ";";
		ss << m_slowTime << ";";
		return ss.str();
	}

	void PlayerControllerComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;

		std::getline(ss, s1, ';');
		m_isActive = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_slowTime = std::stof(s1);
	}

}