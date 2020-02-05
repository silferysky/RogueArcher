#include "Precompiled.h"
#include "Checkpoint.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	Checkpoint::Checkpoint(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void Checkpoint::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			PlayerStatusManager::instance().SetCheckpoint(g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity).GetPosition());
		}
	}
}