#include "Precompiled.h"
#include "TeleAnimation.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	TeleAnimation::TeleAnimation(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge)
		: ScriptComponent(entity, logicComponent, statsComponent), m_teleCharge{ teleCharge }
	{
	}

	void TeleAnimation::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void TeleAnimation::AIIdleUpdate()
	{
		if (m_teleCharge > PlayerStatusManager::instance().GetTeleportCharge())
		{
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsAnimating(true);
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setCurrentFrame(0);

			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity);

			glm::vec4 colourFilter = sprite.getFilter();
			colourFilter.a = 1.0f;
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);

			if (g_engine.m_coordinator.ComponentExists<ChildComponent>(m_entity))
			{
				g_engine.m_coordinator.GetComponent<ChildComponent>(m_entity).SetIsFollowing(false);
				g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity).setPosition(
					g_engine.m_coordinator.GetComponent<TransformComponent>(g_engine.m_coordinator.GetComponent<ChildComponent>(m_entity).GetParent()).GetPosition());
			}

		}
		else if (!g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).getIsAnimating() && g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter().a)
			// not animating and not transparent
		{
			glm::vec4 colourFilter = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter();
			colourFilter.a = 0.0f;
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);
		}

		m_teleCharge = PlayerStatusManager::instance().GetTeleportCharge();
	}

	void TeleAnimation::SetTeleCharge(float max)
	{
		m_teleCharge = max;
	}

	float TeleAnimation::GetTeleCharge() const
	{
		return m_teleCharge;
	}
}