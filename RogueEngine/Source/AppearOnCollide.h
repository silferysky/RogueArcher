#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class AppearOnCollide
		: public ScriptComponent
	{
	public:
		AppearOnCollide(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void OnTriggerEnter(Entity other) override;
		virtual void OnTriggerExit(Entity other) override;
	};
}
