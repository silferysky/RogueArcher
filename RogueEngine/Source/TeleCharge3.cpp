/* Start Header ************************************************************************/
/*!
\file           TeleCharge3.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for TeleCharge3

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "TeleCharge3.h"
#include "PlayerStatusManager.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	TeleCharge3::TeleCharge3(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge)
		: ScriptComponent(entity, logicComponent, statsComponent), m_teleCharge{ teleCharge }
	{
	}

	void TeleCharge3::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
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

				if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
				{
					fade->get().setIsActive(true);
					fade->get().setIsFadingIn(false);
				}
				return;
			}
		}

		m_timer = 0.0f;

		m_teleCharge = PlayerStatusManager::instance().GetTeleportCharge();

		if (m_teleCharge < 3)
		{
			if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
			{
				fade->get().setIsActive(true);
				fade->get().setIsFadingIn(false);
			}
		}
		else
		{
			if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
			{
				fade->get().setIsActive(true);
				fade->get().setIsFadingIn(true);
			}
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