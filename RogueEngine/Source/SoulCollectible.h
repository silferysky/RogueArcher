#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class SoulCollectible
		: public ScriptComponent
	{
	public:
		SoulCollectible(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void OnTriggerEnter(Entity other) override;
	private:
	};
}
