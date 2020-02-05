#include "Precompiled.h"
#include "DisappearOnCollide.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	DisappearOnCollide::DisappearOnCollide(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void DisappearOnCollide::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(false);
		}
	}

	void DisappearOnCollide::OnTriggerExit(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(true);
		}
	}

}