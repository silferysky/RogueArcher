#include "Precompiled.h"
#include "StatsComponent.h"

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

	void StatsComponent::SetIsPatrolling(bool patrol)
	{
		m_isPatrolling = patrol;
	}

	bool StatsComponent::GetIsPatrolling()
	{
		return m_isPatrolling;
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

	void StatsComponent::setWaypoint(Vec2& loc, Vec2& newPos)
	{
		std::swap(loc, newPos);
	}

	void StatsComponent::SetTransitionLevel(const char* nextLevelStr)
	{
		m_nextLevel = nextLevelStr;
	}

	std::string& StatsComponent::GetTransitionLevel()
	{
		return m_nextLevel;
	}

	std::string StatsComponent::Serialize()
	{
		//Health, speed, attack range, sight range
		std::ostringstream ss;
		ss << m_health << ";";
		ss << m_speed << ";";
		ss << m_sightRange << ";";
		ss << m_attackRange << ";";
		ss << m_isPatrolling << ";";
		ss << m_nextLevel << ";";
		ss << m_waypoints.size() << ";";

		for (Vec2& vec : m_waypoints)
		{
			ss << vec.x << ";" << vec.y << ";";
		}

		return ss.str();
	}

	void StatsComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1, s2;

		std::getline(ss, s1, ';');
		m_health = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_speed = std::stof(s1);
		std::getline(ss, s1, ';');
		m_sightRange = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_attackRange = std::stoi(s1);
		std::getline(ss, s1, ';');
		m_isPatrolling = std::stoi(s1);
		std::getline(ss, s1, ';');
		SetTransitionLevel(s1.c_str());

		std::getline(ss, s1, ';');
		size_t count = std::stoi(s1);

		for (size_t it = 0; it < count; ++it)
		{
			std::getline(ss, s1, ';');
			std::getline(ss, s2, ';');
			m_waypoints.push_back(Vec2(std::stof(s1), std::stof(s2)));
		}
		
	}

	void StatsComponent::DisplayOnInspector()
	{
		ImGui::PushItemWidth(75);
		ImGui::DragInt("Health", &m_health, 1.0f, 0, 10);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Speed", &m_speed, 1.0f, 0.0f, 10.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragInt("Attack Range", &m_attackRange, 1.0f, 0, 1000);

		ImGui::PushItemWidth(75);
		ImGui::DragInt("Sight Range", &m_sightRange, 1, 0, 1000);

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Patrolling", &m_isPatrolling);

		static char newLevelPath[128];
		ImGui::PushItemWidth(75);
		ImGui::TextWrapped("Next Transition Level");
		ImGui::TextWrapped("%s", m_nextLevel.c_str());
		ImGui::TextWrapped("Transition Level Name");
		ImGui::SameLine();
		ImGui::PushItemWidth(200);
		ImGui::InputText("                      ", newLevelPath, 128);

		if (ImGui::Button("Set New Transition Level"))
		{
			SetTransitionLevel(newLevelPath);
		}

		std::ostringstream ostrstream;
		size_t count = 1;
		for (Vec2& waypoint : m_waypoints)
		{
			ostrstream.clear();
			ostrstream.str("");
			ostrstream << "Waypoint " << count << " X";
			ImGui::PushItemWidth(75);
			ImGui::DragFloat(ostrstream.str().c_str(), &waypoint.x, 1.0f, -10000.0f, 10000.0f);
			ostrstream.clear();
			ostrstream.str("");
			ostrstream << "Waypoint " << count << " Y";
			ImGui::PushItemWidth(75);
			ImGui::DragFloat(ostrstream.str().c_str(), &waypoint.y, 1.0f, -10000.0f, 10000.0f);
			++count;
		}

		if (ImGui::Button("Add Waypoint"))
		{
			m_waypoints.push_back(Vec2());
		}

		if (ImGui::Button("Remove Waypoint"))
		{
			if (m_waypoints.size())
				m_waypoints.pop_back();
		}
	}
}