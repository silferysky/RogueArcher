#include "Precompiled.h"
#include "ActivateOnEla.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ActivateOnEla::ActivateOnEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void ActivateOnEla::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ActivateOnEla::AIIdleUpdate()
	{
		if (m_isTeleporting != PlayerStatusManager::instance().GetIsTeleporting())
		{
			m_isTeleporting = PlayerStatusManager::instance().GetIsTeleporting();

			if (auto particle = g_engine.m_coordinator.TryGetComponent<ParticleEmitterComponent>(m_entity))
			{
				if (!m_isLightMode && !m_isTeleporting)
				{
					particle->get().SetIsActive(true);
				}
				else if (particle->get().GetIsActive())
					// active
				{
					particle->get().SetIsActive(false);
				}
			}
		}

		if (m_isLightMode != PlayerStatusManager::instance().GetLightStatus())
		{
			m_isLightMode = PlayerStatusManager::instance().GetLightStatus();

			if (auto particle = g_engine.m_coordinator.TryGetComponent<ParticleEmitterComponent>(m_entity))
			{
				if (!m_isLightMode)
				{
					particle->get().SetIsActive(true);
				}
				else if (particle->get().GetIsActive())
					// active
				{
					particle->get().SetIsActive(false);
				}
			}
		}
	}
}