#include "Precompiled.h"
#include "DisappearOnCollide.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	DisappearOnCollide::DisappearOnCollide(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void DisappearOnCollide::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
			{
				fade->get().setIsActive(true);
				fade->get().setIsFadingIn(false);
			}
		}
	}

	void DisappearOnCollide::OnTriggerExit(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
			{
				fade->get().setIsActive(true);
				fade->get().setIsFadingIn(true);
			}
		}
	}

}