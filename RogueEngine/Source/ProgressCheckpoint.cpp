/* Start Header ************************************************************************/
/*!
\file           ProgressCheckpoint.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for ProgressCheckpoint

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Precompiled.h"
#include "ProgressCheckpoint.h"
#include "PlayerStatusManager.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ProgressCheckpoint::ProgressCheckpoint(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_completed{false}
	{
	}

	void ProgressCheckpoint::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ProgressCheckpoint::AIIdleUpdate()
	{
		if (m_completed) // no need to check once its finished
			return;

		float completionPercentage = 0.0f;

		// guard against 0 division
		if (PLAYER_STATUS.GetTotalSoulsInLevel())
			completionPercentage = PLAYER_STATUS.GetCollectedSoulsInLevel() / static_cast<float>(PLAYER_STATUS.GetTotalSoulsInLevel());

		//std::cout << "Completion:" << completionPercentage << std::endl;

		if (completionPercentage >= m_statsComponent->getSpeed()/100) // completed
		{
			if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
			{
				sprite->get().setFilter(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
			}
			if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
			{
				fade->get().setIsActive(true);
				fade->get().setIsFadingIn(true);
				m_completed = true;
			}
		}
	}
}