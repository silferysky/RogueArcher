#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class CoralClam
		: public ScriptComponent
	{
	public:
		CoralClam(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	private:
		float m_timer;
		int m_open;
	};
}
