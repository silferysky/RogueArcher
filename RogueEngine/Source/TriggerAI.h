#pragma once
#include "BaseAI.h"

namespace Rogue
{
	class TriggerAI
		: public BaseAI
	{
		bool m_isTriggered;
	public:
		TriggerAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
	
		virtual void OnTriggerEnter(Entity other) override;
	};
}