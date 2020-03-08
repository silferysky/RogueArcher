#include "Precompiled.h"
#include "ActivateOnDeathExa.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ActivateOnDeathExa::ActivateOnDeathExa(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void ActivateOnDeathExa::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ActivateOnDeathExa::AIIdleUpdate()
	{
		if (auto particle = g_engine.m_coordinator.TryGetComponent<ParticleEmitterComponent>(m_entity))
		{
			if (PLAYER_STATUS.GetDeath() && PLAYER_STATUS.GetLightStatus())
				particle->get().SetIsActive(true);
			else
				particle->get().SetIsActive(false);
		}
	}
}