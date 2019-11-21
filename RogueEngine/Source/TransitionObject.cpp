#include "Precompiled.h"
#include "TransitionObject.h"
#include "Main.h"

namespace Rogue
{
	TransitionObject::TransitionObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, std::string levelToLoad)
		: BaseAI(entity, logicComponent, statsComponent), m_levelToLoad{ levelToLoad } {}

	void TransitionObject::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			SceneManager::instance().LoadLevel(m_levelToLoad.c_str());
		}
	}
}