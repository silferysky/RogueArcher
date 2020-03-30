#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class ProgressCheckpoint
		: public ScriptComponent
	{
	public:
		ProgressCheckpoint(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	private:
		bool m_completed;
	};
}
