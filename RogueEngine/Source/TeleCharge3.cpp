#include "Precompiled.h"
#include "TeleCharge3.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	TeleCharge3::TeleCharge3(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge)
		: ScriptComponent(entity, logicComponent, statsComponent), m_teleCharge{ teleCharge }
	{
	}

	void TeleCharge3::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void TeleCharge3::AIIdleUpdate()
	{
		if (m_teleCharge == PlayerStatusManager::instance().GetTeleportCharge())
		{
			if (m_timer < 5.0f)
			{
				m_timer += g_deltaTime * g_engine.GetTimeScale();
				return;
			}
			else
			{
				m_timer = 0.0f;
				g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
				g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(false);
				return;
			}
		}

		m_timer = 0.0f;

		m_teleCharge = PlayerStatusManager::instance().GetTeleportCharge();

		if (m_teleCharge < 3)
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(false);
		}
		else
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(true);
		}
	}

	void TeleCharge3::SetTeleCharge(float max)
	{
		m_teleCharge = max;
	}

	float TeleCharge3::GetTeleCharge() const
	{
		return m_teleCharge;
	}
}