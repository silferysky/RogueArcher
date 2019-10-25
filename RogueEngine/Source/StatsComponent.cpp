#include "StatsComponent.h"
#include <sstream>

namespace Rogue
{
	void StatsComponent::setHealth(int hp)
	{
		m_health = hp;
	}

	int StatsComponent::getHealth() const
	{
		return m_health;
	}

	bool StatsComponent::damaged(int damageVal)
	{
		return (m_health -= damageVal) <= 0;
	}

	bool StatsComponent::getAlive() const
	{
		return m_health > 0;
	}

	void StatsComponent::setSpeed(float spd)
	{
		m_speed = spd;
	}

	float StatsComponent::getSpeed() const
	{
		return m_speed;
	}

	void StatsComponent::setAttackRange(int atkRange)
	{
		m_attackRange = atkRange;
	}

	int StatsComponent::getAttackRange() const
	{
		return m_attackRange;
	}

	void StatsComponent::setSightRange(int sightRange)
	{
		m_sightRange = sightRange;
	}

	int StatsComponent::getSightRange() const
	{
		return m_sightRange;
	}

	std::string StatsComponent::Serialize()
	{
		//Health, speed, attack range, sight range
		std::ostringstream ss;
		ss << m_health << ";";
		ss << m_speed << ";";
		ss << m_sightRange << ";";
		ss << m_attackRange;

		return ss.str();
	}

	void StatsComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;
		int counter = 0;		//Needed to take in for multiple values

		std::getline(ss, s1, ';');
		m_health = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_speed = std::stof(s1);
		std::getline(ss, s1, ';');
		m_sightRange = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_attackRange = std::stoi(s1);
		
	}
}