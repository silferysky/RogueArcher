#include "Precompiled.h"
#include "AppearOnCollide.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	AppearOnCollide::AppearOnCollide(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void AppearOnCollide::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(true);
		}
	}

	void AppearOnCollide::OnTriggerExit(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(false);
		}
	}

}