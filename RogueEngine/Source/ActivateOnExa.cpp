/* Start Header ************************************************************************/
/*!
\file           ActivateOnExa.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for ActivateOnExa

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "ActivateOnExa.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ActivateOnExa::ActivateOnExa(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void ActivateOnExa::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ActivateOnExa::AIIdleUpdate()
	{
		if (m_isTeleporting != PlayerStatusManager::instance().GetIsTeleporting())
		{
			m_isTeleporting = PlayerStatusManager::instance().GetIsTeleporting();

			if (auto particle = g_engine.m_coordinator.TryGetComponent<ParticleEmitterComponent>(m_entity))
			{
				if (m_isLightMode && !m_isTeleporting)
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
				if (m_isLightMode)
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
