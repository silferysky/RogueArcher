#include "Precompiled.h"
#include "ProgressBar.h"
#include "PlayerStatusManager.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	ProgressBar::ProgressBar(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_oldScale{}
	{
	}

	void ProgressBar::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void ProgressBar::AIIdleUpdate()
	{
		if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
		{
			float completionPercentage = PlayerStatusManager::instance().GetSoulsCollected() / 2.0f;

			int width = GetWindowWidth(g_engine.GetWindowHandler());

			float pixelRatio = (565.0f / static_cast<float>(GetWindowWidth(g_engine.GetWindowHandler()))); // ratio of the asset in comparison to the screen

			transform->get().setScale(Vec2(282.5f * completionPercentage, transform->get().GetScale().y));

			float difference = pixelRatio * (completionPercentage - m_oldScale) / 2.0f;

			// Bar offset
			if (difference > 0)
				transform->get().setPosition(Vec2(transform->get().GetPosition().x - difference, transform->get().GetPosition().y));
			else if (difference < 0)
				transform->get().setPosition(Vec2(transform->get().GetPosition().x + difference, transform->get().GetPosition().y));

			m_oldScale = completionPercentage; // keep track of old scale
		}
	}
}