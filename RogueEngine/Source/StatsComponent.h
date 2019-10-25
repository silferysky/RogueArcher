#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class StatsComponent
		: public BaseComponent
	{
	public:
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

		//From BaseComponent
		std::string Serialize();
		void Deserialize(std::string toDeserialize);

	private:
		//Base Stats
		int m_health;
		float m_speed;

		//Range
		int m_attackRange;	//Max range of attack
		int m_sightRange;	//Max range of detection

	};
}