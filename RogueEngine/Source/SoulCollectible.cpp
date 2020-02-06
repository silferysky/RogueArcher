#include "Precompiled.h"
#include "SoulCollectible.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	SoulCollectible::SoulCollectible(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void SoulCollectible::OnTriggerEnter(Entity other)
	{
		if (!m_collected)
		{
			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
			{
				if (!g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter().a)
				{
					g_engine.m_coordinator.AddToDeleteQueue(m_entity);
					return;
				}

				g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
				g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(false);

				g_engine.m_coordinator.loadSound("Resources/Sounds/soul_pickup.ogg", 0.3f, false).Play();
				PlayerStatusManager::instance().IncrementSoulsCollected();
			}

			m_collected = true;
		}
	}
}