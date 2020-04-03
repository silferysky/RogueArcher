/* Start Header ************************************************************************/
/*!
\file           ActivateOnDeathEla.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for ActivateOnDeathEla

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "ActivateOnDeathEla.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ActivateOnDeathEla::ActivateOnDeathEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void ActivateOnDeathEla::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ActivateOnDeathEla::AIIdleUpdate()
	{
		if (auto particle = g_engine.m_coordinator.TryGetComponent<ParticleEmitterComponent>(m_entity))
		{
			if (PLAYER_STATUS.GetDeath() && !PLAYER_STATUS.GetLightStatus())
				particle->get().SetIsActive(true);
			else
				particle->get().SetIsActive(false);
		}
	}
}