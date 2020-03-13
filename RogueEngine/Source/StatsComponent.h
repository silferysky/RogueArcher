/* Start Header ************************************************************************/
/*!
\file           StatsComponent.h
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
#pragma once
#include "BaseComponent.h"
#include <vector>
#include <string>
#include "Vector2D.h"
#include "GameLevels.h"

namespace Rogue
{
	class StatsComponent
		: public BaseComponent
	{
	public:
		StatsComponent() = default;
		~StatsComponent() = default;

		void setHealth(int hp);
		int getHealth() const;
		bool damaged(int damageVal);
		bool getAlive() const;

		void setSpeed(float spd);
		float getSpeed() const;

		void setAttackRange(int atkRange);
		int getAttackRange() const;

		void setSightRange(int sightRange);
		int getSightRange() const;

		void SetIsPatrolling(bool patrol);
		bool GetIsPatrolling();

		void addWaypoint(Vec2& vec);
		std::vector<Vec2> getWaypoints() const;
		void clearWaypoints();
		void setWaypoint(Vec2& loc, Vec2& newPos);

		void SetTransitionLevel(const char* nextLevelStr);
		std::string& GetTransitionLevel();

		void SetZoomValue(float zoom);
		float GetZoomValue() const;
		void SetZoomDuration(float dur);
		float GetZoomDuration() const;
		void SetZoomDelay(float delay);
		float GetZoomDelay() const;
		void SetZoomCount(unsigned count);
		unsigned GetZoomCount() const;

		void SetEmojiDelay(float delay);
		float GetEmojiDelay() const;
		void AddEmojiTexture(std::string texturePath);
		std::vector<std::string> GetEmojiTextures() const;

		void setLevel(LEVEL level);
		LEVEL getLevel() const;

		//From BaseComponent
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();

	private:
		//Base Stats
		int m_health;
		float m_speed;

		//Range
		int m_attackRange;	//Max range of attack
		int m_sightRange;	//Max range of detection

		//To enable Patrol State
		bool m_isPatrolling;

		std::vector<Vec2> m_waypoints;	//Waypoints if need to patrol
		std::string m_nextLevel;

		//For camera zoom
		float m_zoomValue;
		float m_zoomDuration;
		float m_zoomDelay;
		unsigned m_zoomCount;

		//For Emojis
		std::vector<std::string> m_emojiTexture;
		float m_emojiDelay;

		LEVEL m_level;
	};
}