#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class AnimateOnEla
		: public ScriptComponent
	{
	public:
		AnimateOnEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	private:
		bool m_isLightMode = true;
	};
}
