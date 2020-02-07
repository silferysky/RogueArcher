#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class ActivateOnDeathEla
		: public ScriptComponent
	{
	public:
		ActivateOnDeathEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	};
}
