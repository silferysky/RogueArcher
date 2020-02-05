#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class DisappearOnCollide
		: public ScriptComponent
	{
	public:
		DisappearOnCollide(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void OnTriggerEnter(Entity other) override;
		virtual void OnTriggerExit(Entity other) override;
	};
}
