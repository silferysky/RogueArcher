#pragma once
#include "BaseAI.h"
#include <string>

namespace Rogue
{
	class TransitionObject
		: public BaseAI
	{
	public:
		TransitionObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, const std::string& levelToLoad = "Level 1.json");

		virtual void OnTriggerEnter(Entity other) override;

	private:
		std::string m_levelToLoad;
	};
}