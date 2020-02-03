#pragma once
#include "PatrolAI.h"

namespace Rogue
{
	class PlatformAI : public PatrolAI
	{
	public:
		PlatformAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
		virtual void AIPatrolUpdate() override;
	};
}