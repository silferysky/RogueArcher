/* Start Header ************************************************************************/
/*!
\file           ProgressBar.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for ProgressBar

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "ProgressBar.h"
#include "PlayerStatusManager.h"
#include "UISystem.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ProgressBar::ProgressBar(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_oldScale{}, m_difference{}
	{
		if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(entity))
		{
			auto& transformOpt = transform->get();
			transformOpt.setScale(Vec2(0.0f, transformOpt.GetScale().y));
		}
	}

	void ProgressBar::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ProgressBar::AIIdleUpdate()
	{
		float completionPercentage = 0.0f;

		// guard against 0 division
		if (PLAYER_STATUS.GetTotalSoulsInLevel())
			completionPercentage = PLAYER_STATUS.GetCollectedSoulsInLevel() / static_cast<float>(PLAYER_STATUS.GetTotalSoulsInLevel());

		std::stringstream ss;
		ss << "completionPercentage: " << completionPercentage
			<< " Curr level: " << PLAYER_STATUS.GetCurrLevel();

		//RE_INFO(ss.str());
		// player - camera -> should - now
		if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
		{
			if (m_oldScale != completionPercentage)
			{

				auto positon = transform->get().GetPosition();

				// recenter the bar by reverting the changes to position
				if (m_difference < 0)
					transform->get().setPosition(Vec2(positon.x - m_difference, positon.y));
				else if (m_difference > 0)
					transform->get().setPosition(Vec2(positon.x + m_difference, positon.y));

				positon = transform->get().GetPosition();

				// translate by half of (1 - completionPercentage)
				m_difference = 282.5f * 0.5f * (1 - completionPercentage);

				// Bar offset
				if (m_difference > 0)
					transform->get().setPosition(Vec2(positon.x - m_difference, positon.y));
				else if (m_difference < 0)
					transform->get().setPosition(Vec2(positon.x + m_difference, positon.y));

				m_oldScale = completionPercentage; // keep track of old scale
			}


			/* auto position = transform->get().GetPosition();
			float deltaPosX = 0.0f;

			// recenter the bar by reverting the changes to position
			if (m_difference < 0)
				deltaPosX = -m_difference;
			else if (m_difference > 0)
				deltaPosX = m_difference;

			position.x += deltaPosX * 1.6f * g_fixedDeltaTime;

			// translate by half of (1 - completionPercentage)
			m_difference = 282.5f * 0.5f * (1 - completionPercentage);

			// Bar offset
			if (m_difference > 0)
				deltaPosX = -m_difference;
			else if (m_difference < 0)
				deltaPosX = m_difference;

			position.x += deltaPosX * 1.6f * g_fixedDeltaTime;

			transform->get().setPosition(position); */

			float scaleX = transform->get().GetScale().x;

			float deltaX = (282.5f * completionPercentage) - scaleX;
			scaleX += static_cast<int>(deltaX) * 1.6f * g_fixedDeltaTime;

			// 282.5 is the scale of the full bar
			transform->get().setScale(Vec2(scaleX, transform->get().GetScale().y));
		}
	}
}