#pragma once
#include "BaseComponent.h"
#include <vector>
#include "Vector2D.h"

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

		void addWaypoint(Vec2& vec);
		std::vector<Vec2> getWaypoints() const;
		void clearWaypoints();
		void setWaypoint(Vec2& loc, Vec2& newPos);

		//From BaseComponent
		std::string_view Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();

	private:
		//Base Stats
		int m_health;
		float m_speed;

		//Range
		int m_attackRange;	//Max range of attack
		int m_sightRange;	//Max range of detection

		std::vector<Vec2> m_waypoints;	//Waypoints if need to patrol

	};
}