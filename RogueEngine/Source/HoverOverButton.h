#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class HoverOverButton
		: public ScriptComponent
	{
	public:
		HoverOverButton(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
	private:
		Vec2 m_originalScale;
	};
}
