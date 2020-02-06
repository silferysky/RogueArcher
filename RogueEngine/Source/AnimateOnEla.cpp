#include "Precompiled.h"
#include "AnimateOnEla.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	AnimateOnEla::AnimateOnEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void AnimateOnEla::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void AnimateOnEla::AIIdleUpdate()
	{
		if (m_isLightMode == PlayerStatusManager::instance().GetLightStatus())
		{
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsAnimating(true);

			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity);

			glm::vec4 colourFilter = sprite.getFilter();
			colourFilter.a = 1.0f;
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);
		}
		else if (!g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).getIsAnimating() && g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter().a)
			// not animating and not transparent
		{
			glm::vec4 colourFilter = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter();
			colourFilter.a = 0.0f;
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);
		}

		m_isLightMode = PlayerStatusManager::instance().GetLightStatus();
	}
}