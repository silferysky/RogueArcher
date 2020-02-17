#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class Checkpoint
		: public ScriptComponent
	{
	public:
		Checkpoint(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void OnTriggerEnter(Entity other) override;
	private:
	};
}
