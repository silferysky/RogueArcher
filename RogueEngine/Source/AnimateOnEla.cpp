#include "Precompiled.h"
#include "AnimateOnEla.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	AnimateOnEla::AnimateOnEla(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void AnimateOnEla::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void AnimateOnEla::AIIdleUpdate()
	{
		if (auto child = g_engine.m_coordinator.TryGetComponent<ChildComponent>(m_entity))
		{
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(child->get().GetParent()))
			{
				if (PLAYER_STATUS.GetMoveLeft())
				{
					child->get().SetScale(Vec2(-1.0f, 1.0f));
				}
				else
				{
					child->get().SetScale(Vec2(1.0f, 1.0f));
				}

				child->get().SetPosition(0.0f, 0.0f);
				child->get().SetGlobalDirty();
			}
		}

		if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
		{
			if (auto animate = g_engine.m_coordinator.TryGetComponent<AnimationComponent>(m_entity))
			{
				if (m_isLightMode != PlayerStatusManager::instance().GetLightStatus())
				{
					m_isLightMode = PlayerStatusManager::instance().GetLightStatus();

					if (!m_isLightMode)
					{
						animate->get().setIsAnimating(true);

						// set parent to disappear
						if (auto optChild = g_engine.m_coordinator.TryGetComponent<ChildComponent>(m_entity))
						{
							Entity parent = optChild->get().GetParent();

							if (auto parentSprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(parent))
							{
								glm::vec4 parentFilter = parentSprite->get().getFilter();
								parentFilter.a = 0.0f;
								parentSprite->get().setFilter(parentFilter);
							}
						}

						glm::vec4 colourFilter = sprite->get().getFilter();
						colourFilter.a = 1.0f;
						sprite->get().setFilter(colourFilter);
					}
				}
				else if (!animate->get().getIsAnimating() && sprite->get().getFilter().a)
					// not animating and not transparent
				{
					//set parent to appear
					if (auto childOpt = g_engine.m_coordinator.TryGetComponent<ChildComponent>(m_entity))
					{
						Entity parent = childOpt->get().GetParent();

						if (auto parentSprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(parent))
						{
							glm::vec4 parentFilter = parentSprite->get().getFilter();
							parentFilter.a = 1.0f;
							parentSprite->get().setFilter(parentFilter);
						}
					}

					glm::vec4 colourFilter = sprite->get().getFilter();
					colourFilter.a = 0.0f;
					sprite->get().setFilter(colourFilter);
				}
			}
		}
	}
}