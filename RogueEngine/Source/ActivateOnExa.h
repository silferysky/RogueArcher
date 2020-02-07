#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class ActivateOnExa
		: public ScriptComponent
	{
	public:
		ActivateOnExa(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	private:
		bool m_isLightMode = true;
	};
}

