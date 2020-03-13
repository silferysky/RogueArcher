#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class DeathBox
		: public ScriptComponent
	{
	public:
		DeathBox(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		virtual void OnTriggerEnter(Entity other) override;
	private:
		float m_age;
		float m_lifetime;
		bool m_triggered;
		int m_playerZ;
	};
}