#include "Precompiled.h"
#include "Checkpoint.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	Checkpoint::Checkpoint(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void Checkpoint::OnTriggerEnter(Entity other)
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
			
			g_engine.m_coordinator.loadSound("Resources/Sounds/soul_pickup.ogg", 0.3f, false).Play();
			if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
				PlayerStatusManager::instance().SetCheckpoint(transform->get().GetPosition());

			g_engine.m_coordinator.AddToDeleteQueue(m_entity);
		}
	}
}