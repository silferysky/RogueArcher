#include "pch.h"
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

	void BackgroundComponent::Deserialize(const std::string& toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;		//s2 is used if two are needed

		while (std::getline(ss, s1, ';'))
		{
			SetParallax(std::stof(s1));
		}
	}
}