#include "Precompiled.h"
#include "SoulCollectible.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	SoulCollectible::SoulCollectible(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void SoulCollectible::OnTriggerEnter(Entity other)
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

			int level = atoi(SceneManager::instance().getCurrentFileName().substr(6, 2).c_str());

			g_engine.m_coordinator.loadSound("Resources/Sounds/soul_pickup.ogg", 0.3f, false).Play();

			PLAYER_STATUS.CollectSoul(m_entity);

			/* if (auto audio = g_engine.m_coordinator.TryGetComponent<AudioEmitterComponent>(m_entity))
			{
				audio->get().DestroySound();
				audio->get().getSound().Unload();
			} */
			g_engine.m_coordinator.AddToDeleteQueue(m_entity);
		}
	}
}