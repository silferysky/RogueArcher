/* Start Header ************************************************************************/
/*!
\file           LightsFlicker.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for LightsFlicker

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "LightsFlicker.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	LightsFlicker::LightsFlicker(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void LightsFlicker::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void LightsFlicker::AIIdleUpdate()
	{
		m_timer += g_deltaTime * g_engine.GetTimeScale();

		if (m_timer > static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(m_statsComponent->getSpeed()/100.0f)))
		{
			if (auto light = g_engine.m_coordinator.TryGetComponent<LightComponent>(m_entity))
			{
				float random = (RAND_MAX / m_statsComponent->getHealth());

				if (random) // guard against division by 0
				{
					light->get().setAmbientFactor(static_cast<float>(rand() / random));
					light->get().setSpecularFactor(static_cast<float>(rand() / random));
				}

				m_timer = 0.0f;
			}
		}
	}
}