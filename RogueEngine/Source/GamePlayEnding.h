#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class GamePlayEnding : public ScriptComponent
	{
		int m_souls;
		float m_timer;
	public:
		GamePlayEnding(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
		virtual void OnTriggerEnter(Entity other) override;
	};
}
