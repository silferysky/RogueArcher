#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class ProgressBar
		: public ScriptComponent
	{
	public:
		ProgressBar(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	private:
		float m_oldScale;
		float m_difference;
	};
}
