#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class ActivateOnDeathExa
		: public ScriptComponent
	{
	public:
		ActivateOnDeathExa(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	};
}
