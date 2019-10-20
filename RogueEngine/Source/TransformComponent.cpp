#include <sstream>
#include "TransformComponent.h"
#include "REMath.h"

namespace Rogue
{
	void TransformComponent::setPosition(const Vec2& pos)
	{
		m_position = pos;
	}

	void TransformComponent::offSetPosition(const Vec2& pos)
	{
		m_position += pos;
	}

	void TransformComponent::setScale(const Vec2& scale)
	{
		m_scale = scale;
	}

	void TransformComponent::offSetScale(const Vec2& scale)
	{
		m_scale += scale;
	}

	void TransformComponent::setRotation(const float rot)
	{
		m_rotation = rot;
	}

	void TransformComponent::offSetRotation(const float rot)
	{
		m_rotation += rot;
	}

	Vec2 TransformComponent::getPosition() const
	{
		return m_position;
	}

	Vec2 TransformComponent::getScale() const
	{
		return m_scale;
	}

	float TransformComponent::getRotation() const
	{
		return m_rotation;
	}

	std::string TransformComponent::Serialize()
	{
		//Position, Scale, Rotation
		std::ostringstream ss;
		ss << m_position.x << ";" << m_position.y << ";";
		ss << m_scale.x << ";" << m_scale.y << ";";
		ss << m_rotation;
		return ss.str();
	}

	void TransformComponent::Deserialize(std::string toDeserialize)
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
			case 1:
				setScale(Vec2(std::stof(s1), std::stof(s2)));
				break;
			case 2:
				setRotation(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}