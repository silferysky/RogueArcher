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

	void StatsComponent::addWaypoint(Vec2& vec)
	{
		m_waypoints.push_back(vec);
	}

	std::vector<Vec2> StatsComponent::getWaypoints() const
	{
		return m_waypoints;
	}

	void StatsComponent::clearWaypoints()
	{
		m_waypoints.clear();
	}

	std::string StatsComponent::Serialize()
	{
		//Health, speed, attack range, sight range
		std::ostringstream ss;
		ss << m_health << ";";
		ss << m_speed << ";";
		ss << m_sightRange << ";";
		ss << m_attackRange << ";";
		ss << m_waypoints.size() << ";";

		for (Vec2 vec : m_waypoints)
		{
			ss << vec.x << ";" << vec.y << ";";
		}

		return ss.str();
	}

	void StatsComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;

		std::getline(ss, s1, ';');
		m_health = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_speed = std::stof(s1);
		std::getline(ss, s1, ';');
		m_sightRange = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_attackRange = std::stoi(s1);
		
	}

	void StatsComponent::DisplayOnInspector()
	{
		ImGui::PushItemWidth(75);
		ImGui::DragInt("Health", &m_health, 1.0f, 0, 10);
		setHealth(m_health);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Speed", &m_speed, 1.0f, 0.0f, 10.0f);
		setSpeed(m_speed);

		ImGui::PushItemWidth(75);
		ImGui::DragInt("Attack Range", &m_attackRange, 1.0f, 0, 1000);
		setAttackRange(m_attackRange);

		ImGui::PushItemWidth(75);
		ImGui::DragInt("Sight Range", &m_sightRange, 1, 0, 1000);
		setSightRange(m_sightRange);

		for (Vec2& waypoint : m_waypoints)
		{
			ImGui::PushItemWidth(75);
			ImGui::DragFloat("Waypoint X", &waypoint.x, 1.0f, -10000.0f, 10000.0f);
			ImGui::PushItemWidth(75);
			ImGui::DragFloat("Waypoint Y", &waypoint.y, 1.0f, -10000.0f, 10000.0f);
		}

		if (ImGui::Button("Add Waypoint"))
		{
			m_waypoints.push_back(Vec2());
		}
	}
}