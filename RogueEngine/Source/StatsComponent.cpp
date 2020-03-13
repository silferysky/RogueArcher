/* Start Header ************************************************************************/
/*!
\file           StatsComponent.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for StatsComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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

	void StatsComponent::SetZoomValue(float zoom)
	{
		m_zoomValue = zoom;
	}

	float StatsComponent::GetZoomValue() const
	{
		return m_zoomValue;
	}

	void StatsComponent::SetZoomDuration(float dur)
	{
		m_zoomDuration = dur;
	}

	float StatsComponent::GetZoomDuration() const
	{
		return m_zoomDuration;
	}

	void StatsComponent::SetZoomDelay(float delay)
	{
		m_zoomDelay = delay;
	}

	float StatsComponent::GetZoomDelay() const
	{
		return m_zoomDelay;
	}

	void StatsComponent::SetZoomCount(unsigned count)
	{
		m_zoomCount = count;
	}

	unsigned StatsComponent::GetZoomCount() const
	{
		return m_zoomCount;
	}

	void StatsComponent::SetEmojiDelay(float delay)
	{
		m_emojiDelay = delay;
	}

	float StatsComponent::GetEmojiDelay() const
	{
		return m_emojiDelay;
	}

	void StatsComponent::AddEmojiTexture(std::string texturePath)
	{
		m_emojiTexture.push_back(texturePath);
	}

	std::vector<std::string> StatsComponent::GetEmojiTextures() const
	{
		return m_emojiTexture;
	}

	void StatsComponent::setLevel(LEVEL level)
	{
		m_level = level;
	}

	LEVEL StatsComponent::getLevel() const
	{
		return m_level;
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

		ss << m_zoomValue << ";";
		ss << m_zoomDuration << ";";
		ss << m_zoomDelay << ";";
		ss << m_zoomCount << ";";

		ss << m_emojiDelay << ";";
		ss << m_emojiTexture.size() << ";";

		for (auto emojiTex : m_emojiTexture)
		{
			ss << emojiTex << ";";
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

		if (std::getline(ss, s1, ';'))
			m_zoomValue = std::stof(s1);
		if (std::getline(ss, s1, ';'))
			m_zoomDuration = std::stof(s1);
		if (std::getline(ss, s1, ';'))
			m_zoomDelay = std::stof(s1);
		if (std::getline(ss, s1, ';'))
			m_zoomCount = std::stoi(s1);


		if (std::getline(ss, s1, ';'))
			m_emojiDelay = std::stof(s1);

		count = 0;
		if (std::getline(ss, s1, ';'))
			count = std::stoi(s1);

		for (size_t i = 0; i < count; ++i)
		{
			if (std::getline(ss, s1, ';'))
				m_emojiTexture.push_back(s1);
		}
	}

	void StatsComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImGui::PushItemWidth(75);
		ImGui::DragInt("Health", &m_health, 1.0f, 0, 10);

		const char* levelNames[] = { "Crystal Caverns", "Amethyst Seas", "Verdant Forest" };
		int tempInt = static_cast<int>(m_level);

		//For Levels
		ImGui::Combo("Level", &tempInt, levelNames, IM_ARRAYSIZE(levelNames));
		m_level = (LEVEL)tempInt;

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Speed", &m_speed, 1.0f, 0.0f, 10.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragInt("Attack Range", &m_attackRange, 1.0f, 0, 1000);

		ImGui::PushItemWidth(75);
		ImGui::DragInt("Sight Range", &m_sightRange, 1, 0, 1000);

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Patrolling", &m_isPatrolling);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Zoom Value", &m_zoomValue, 1.0f, 0.0f, 1000.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Zoom Duration", &m_zoomDuration, 1.0f, 0.0f, 10.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Zoom Delay", &m_zoomDelay, 1.0f, 0.0f, 1000.0f);

		int tempZoomCount = static_cast<int>(m_zoomCount);
		ImGui::PushItemWidth(75);
		ImGui::DragInt("Zoom Count", &tempZoomCount, 1.0f, 0, 20);
		m_zoomCount = static_cast<unsigned>(tempZoomCount);

		std::ostringstream ostrstream;
		size_t count = 1;

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Emoji Delay", &m_emojiDelay, 1.0f, 0, 100);

		for (auto& emojiStr : m_emojiTexture)
		{
			ostrstream.clear();
			ostrstream.str("");
			ostrstream << "Emoji Texture " << count << ": " << emojiStr;
			ImGui::TextWrapped(ostrstream.str().c_str());
				
			++count;
		}

		static char emojiPath[64];
		ImGui::TextWrapped("Emoji Texture");
		ImGui::SameLine();
		ImGui::InputText("                    ", emojiPath, 128);
		ImGui::PushItemWidth(75);

		if (ImGui::Button("Add new Emoji Texture"))
		{
			AddEmojiTexture(emojiPath);
		}
		
		if (m_emojiTexture.size())
			if (ImGui::Button("Remove last Emoji Texture"))
			{ 
				m_emojiTexture.pop_back();
			}


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

		ostrstream.clear();
		ostrstream.str("");
		count = 1;
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