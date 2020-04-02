#pragma once
#include "ScriptComponent.h"
#include <string>

namespace Rogue
{
	class TransitionScreen
		: public ScriptComponent
	{
	public:
		TransitionScreen(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, const std::string& levelToLoad = "Level 1.json");

		virtual void AIActiveStateUpdate() override;

		std::string& GetTransitionLevelName();
		void SetTransitionLevelName(std::string name);

	private:
		std::string m_levelToLoad;
		float m_timer = 0.0f;
	};
}