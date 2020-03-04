#include "Precompiled.h"
#include "ActivateOnDeathEla.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ActivateOnDeathEla::ActivateOnDeathEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void ActivateOnDeathEla::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ActivateOnDeathEla::AIIdleUpdate()
	{
		if (PLAYER_STATUS.GetDeath() && !PLAYER_STATUS.GetLightStatus())
			if (auto optParticle = g_engine.m_coordinator.TryGetComponent<ParticleEmitterComponent>(m_entity))
			{
				optParticle->get().SetIsActive(true);
			}
		else
			if(auto optParticle = g_engine.m_coordinator.TryGetComponent<ParticleEmitterComponent>(m_entity))
			{
				optParticle->get().SetIsActive(false);
			}
	}
}