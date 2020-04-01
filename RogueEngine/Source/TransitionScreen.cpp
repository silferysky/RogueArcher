#include "Precompiled.h"
#include "TransitionScreen.h"
//#include "Main.h"
#include "GraphicsEvent.h"
#include "InputManager.h"

namespace Rogue
{
	TransitionScreen::TransitionScreen(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, const std::string& levelToLoad)
		: ScriptComponent(entity, logicComponent, statsComponent), m_levelToLoad{ levelToLoad } {}

	void TransitionScreen::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		// increment timer
		m_timer += g_deltaTime * g_engine.GetTimeScale();

		if (m_timer >= m_statsComponent->getHealth() || g_engine.m_coordinator.GetSystem<InputManager>()->KeyDown(KeyPress::MB1))
		{
			g_engine.m_coordinator.SetTransitionLevel(m_levelToLoad.c_str(), 0.0f); //2nd value doesn't matter anymore probably

			FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f);
			ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
			EventDispatcher::instance().AddEvent(ev);
		}
	}
	std::string& TransitionScreen::GetTransitionLevelName()
	{
		return m_levelToLoad;
	}
	void TransitionScreen::SetTransitionLevelName(std::string name)
	{
		m_levelToLoad = name;
	}
}