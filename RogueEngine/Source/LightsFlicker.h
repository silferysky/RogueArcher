#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class LightsFlicker
		: public ScriptComponent
	{
	public:
		LightsFlicker(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	private:
		float m_timer = 0.0f;
	};
}
