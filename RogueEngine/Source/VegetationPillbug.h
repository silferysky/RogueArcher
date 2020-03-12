#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class VegetationPillbug
		: public ScriptComponent
	{
	public:
		VegetationPillbug(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	};
}
