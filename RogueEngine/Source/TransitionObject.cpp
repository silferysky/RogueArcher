#include "Precompiled.h"
#include "TransitionObject.h"
#include "Main.h"

namespace Rogue
{
	TransitionObject::TransitionObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, const std::string& levelToLoad)
		: BaseAI(entity, logicComponent, statsComponent), m_levelToLoad{ levelToLoad } {}

	void TransitionObject::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			SceneManager& sceneManager = SceneManager::instance();

			sceneManager.LoadLevel(sceneManager.getCurrentFileName().c_str());
		}
	}
}