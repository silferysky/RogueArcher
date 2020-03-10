#include "Precompiled.h"
#include "ProgressBar.h"
#include "PlayerStatusManager.h"
#include "UISystem.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ProgressBar::ProgressBar(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_oldScale{}
	{
		if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(entity))
		{
			transform->get().setScale(Vec2(0.0f, transform->get().GetScale().y));
		}
	}

	void ProgressBar::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ProgressBar::AIIdleUpdate()
	{
		float completionPercentage = PlayerStatusManager::instance().GetSoulsCollected() / 4.0f;

		if (m_oldScale != completionPercentage)
		{
			if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
			{
				auto positon = transform->get().GetPosition();

				// 282.5 is the scale of the full bar
				transform->get().setScale(Vec2(282.5f * completionPercentage, transform->get().GetScale().y));

				// recenter the bar by reverting the changes to position
				if (m_difference < 0)
					transform->get().setPosition(Vec2(positon.x - m_difference, positon.y));
				else if (m_difference > 0)
					transform->get().setPosition(Vec2(positon.x + m_difference, positon.y));

				// translate by half of (1 - completionPercentage)
				m_difference = 282.5f * 0.5f * (1 - completionPercentage);

				// Bar offset
				if (m_difference > 0)
					transform->get().setPosition(Vec2(positon.x - m_difference, positon.y));
				else if (m_difference < 0)
					transform->get().setPosition(Vec2(positon.x + m_difference, positon.y));

				m_oldScale = completionPercentage; // keep track of old scale
			}
		}
	}
}