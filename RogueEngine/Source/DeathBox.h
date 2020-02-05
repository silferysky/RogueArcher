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
		float m_timer;
		bool m_isDead;
	};
}