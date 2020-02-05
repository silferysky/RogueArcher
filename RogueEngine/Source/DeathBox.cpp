#include "Precompiled.h"
#include "DeathBox.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	DeathBox::DeathBox(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void DeathBox::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void DeathBox::AIIdleUpdate()
	{
		if (m_isDead)
		{
			m_timer += g_deltaTime * g_engine.GetTimeScale();
			if (m_timer >= 0.65f && m_timer < 3.0f)
			{
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
				g_engine.m_coordinator.GetComponent<TransformComponent>(m_other).setPosition(PlayerStatusManager::instance().GetCheckpoint());
			}
			else if (m_timer >= 3.0f)
			{
				m_timer = 0.0f;
				m_isDead = false;
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);
			}
		}
	}

	void DeathBox::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			//g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
			m_isDead = true;
			m_other = other;
		}
	}
}