#include "Precompiled.h"
#include "TeleAnimation.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	TeleAnimation::TeleAnimation(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge)
		: ScriptComponent(entity, logicComponent, statsComponent), m_teleCharge{ teleCharge }
	{
	}

	void TeleAnimation::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void TeleAnimation::AIIdleUpdate()
	{
		if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
		{
			if (auto animation = g_engine.m_coordinator.TryGetComponent<AnimationComponent>(m_entity))
			{
				if (m_teleCharge > PlayerStatusManager::instance().GetTeleportCharge())
				{
					animation->get().setIsAnimating(true);
					animation->get().setCurrentFrame(0);


					glm::vec4 colourFilter = sprite->get().getFilter();
					colourFilter.a = 1.0f;
					g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);

					if (auto child = g_engine.m_coordinator.TryGetComponent<ChildComponent>(m_entity))
					{
						child->get().SetIsFollowing(false);
						if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
							if (auto parentTransform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(child->get().GetParent()))
								transform->get().setPosition(parentTransform->get().GetPosition());
					}

				}
				else if (!animation->get().getIsAnimating() && sprite->get().getFilter().a)
					// not animating and not transparent
				{
					glm::vec4 colourFilter = sprite->get().getFilter();
					colourFilter.a = 0.0f;
					sprite->get().setFilter(colourFilter);
				}
			}
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