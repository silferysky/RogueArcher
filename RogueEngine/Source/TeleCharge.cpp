#include "Precompiled.h"
#include "TeleCharge.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	TeleCharge::TeleCharge(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge)
		: ScriptComponent(entity, logicComponent, statsComponent), m_teleCharge{ teleCharge }
	{
	}

	void TeleCharge::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void TeleCharge::AIIdleUpdate()
	{
		if (m_teleCharge == PlayerStatusManager::instance().GetTeleportCharge())
			return;

		m_teleCharge = PlayerStatusManager::instance().GetTeleportCharge();

		if (m_teleCharge < 3)
		{
			glm::vec4 colourFilter = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter();
			colourFilter.a = 0;
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);
		}
		else
		{
			glm::vec4 colourFilter = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter();
			colourFilter.a = 255;
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);
		}

		/* if (m_teleCharge < 3)
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(false);
		}
		else
		{
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsActive(true);
			g_engine.m_coordinator.GetComponent<FadeComponent>(m_entity).setIsFadingIn(true);
		} */
	}

	void TeleCharge::SetTeleCharge(float max)
	{
		m_teleCharge = max;
	}

	float TeleCharge::GetTeleCharge() const
	{
		return m_teleCharge;
	}
}