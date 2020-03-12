#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class GamePlayEnding : public ScriptComponent
	{
		int m_souls = 0;
		float m_timer = 0.0f;
		bool m_activated = false;
	public:
		GamePlayEnding(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
		virtual void OnTriggerEnter(Entity other) override;
	};
}
