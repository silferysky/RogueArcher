#include "Precompiled.h"
#include "TriggerAI.h"

namespace Rogue
{
	TriggerAI::TriggerAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: BaseAI(entity, logicComponent, statsComponent) {}

	void TriggerAI::OnTriggerEnter(Entity other)
	{
	}
}