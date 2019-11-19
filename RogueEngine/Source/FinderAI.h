#pragma once
#include "BaseAI.h"

namespace Rogue
{
	class FinderAI
		: public BaseAI
	{
	public:
		FinderAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;

		virtual void AIDetect() override;

		virtual void AIChaseUpdate() override;

		virtual void AIIdleUpdate() override;

	};

}