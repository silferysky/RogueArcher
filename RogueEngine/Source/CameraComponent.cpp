#include <sstream>
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

	std::string CameraComponent::Serialize()
	{
		//Position
		std::ostringstream ss;
		ss << m_position.x << ";" << m_position.y << ";";
		return ss.str();
	}

	void CameraComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1, s2;		//s2 is used if two are needed
		int counter = 0;		//Needed to take in for multiple values
		int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1 parameter to set

		while (std::getline(ss, s1, ';'))
		{
			//If counter has not cleared all sets yet, read into s2
			if (counter < sets)
				std::getline(ss, s2, ';');

			switch (counter)
			{
			case 0:
				setPosition(Vec2(std::stof(s1), std::stof(s2)));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}