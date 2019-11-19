#pragma once
#include "BaseAI.h"

namespace Rogue
{
	class TriggerAI
		: public BaseAI
	{
	public:
		TriggerAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

	private:
		virtual void OnTriggerEnter(Entity other) override;
	};
}