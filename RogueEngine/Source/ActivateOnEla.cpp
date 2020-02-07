#include "Precompiled.h"
#include "ActivateOnEla.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ActivateOnEla::ActivateOnEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void ActivateOnEla::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ActivateOnEla::AIIdleUpdate()
	{
		if (m_isLightMode != PlayerStatusManager::instance().GetLightStatus())
		{
			m_isLightMode = PlayerStatusManager::instance().GetLightStatus();

			if (!m_isLightMode)
			{
				g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(m_entity).SetIsActive(true);
			}
			else if (g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(m_entity).GetIsActive())
				// active
			{
				g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(m_entity).SetIsActive(false);
			}
		}
	}
}