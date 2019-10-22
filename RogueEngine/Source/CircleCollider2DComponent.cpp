#include <sstream>
#include "CircleCollider2DComponent.h"


namespace Rogue
{
	float CircleCollider2DComponent::getRadius() const
	{
		return m_radius;
	}

	float CircleCollider2DComponent::getCenter() const
	{
		return m_center;
	}

	void CircleCollider2DComponent::setRadius(float radius)
	{
		m_radius = radius;
	}

	void CircleCollider2DComponent::setCenter(float center)
	{
		m_center = center;
	}

	std::string CircleCollider2DComponent::Serialize()
	{
		//Radius
		std::ostringstream ss;
		ss << m_radius;

		return ss.str();
	}

	void CircleCollider2DComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;			//s2 is used if two are needed
		//int counter = 0;		//Needed to take in for multiple values
		//int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1 parameter to set

		while (std::getline(ss, s1, ';'))
		{
			m_radius = std::stof(s1);
		}
	}
}