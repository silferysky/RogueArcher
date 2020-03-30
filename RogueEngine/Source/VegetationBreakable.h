#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class VegetationBreakable
		: public ScriptComponent
	{
	public:
		VegetationBreakable(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		virtual void OnTriggerEnter(Entity other) override;
	private:
		float m_timer = 0.0f;
		bool m_triggered = false;
	};
}
