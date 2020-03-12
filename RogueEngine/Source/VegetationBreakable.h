#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class VegetationBreakable
		: public ScriptComponent
	{
	public:
		VegetationBreakable(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void OnTriggerEnter(Entity other) override;
	};
}
