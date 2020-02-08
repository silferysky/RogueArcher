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
		if (g_engine.m_coordinator.ComponentExists<ChildComponent>(m_entity))
		{
			auto& child = g_engine.m_coordinator.GetComponent<ChildComponent>(m_entity);
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(child.GetParent()))
			{
				if (PLAYER_STATUS.GetMoveLeft())
				{
					child.SetScale(Vec2(-1.0f, 1.0f));
				}
				else
				{
					child.SetScale(Vec2(1.0f, 1.0f));
				}

				child.SetPosition(0.0f, 0.0f);
				child.SetPositionZ(1);
				child.SetGlobalDirty();
			}
		}

		if (m_isLightMode != PlayerStatusManager::instance().GetLightStatus())
		{
			m_isLightMode = PlayerStatusManager::instance().GetLightStatus();

			if (!m_isLightMode)
			{
				g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setCurrentFrame(0);
				g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsAnimating(true);

				// set parent to disappear
				Entity m_parent = g_engine.m_coordinator.GetComponent<ChildComponent>(m_entity).GetParent();
				auto& parentSprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_parent);
				glm::vec4 parentFilter = parentSprite.getFilter();
				parentFilter.a = 0.0f;
				parentSprite.setFilter(parentFilter);

				auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity);
				glm::vec4 colourFilter = sprite.getFilter();
				colourFilter.a = 1.0f;
				sprite.setFilter(colourFilter);

			}
		}
		else if (!g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).getIsAnimating() && g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter().a)
			// not animating and not transparent
		{
			//set parent to appear
			Entity m_parent = g_engine.m_coordinator.GetComponent<ChildComponent>(m_entity).GetParent();
			auto& parentSprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_parent);
			glm::vec4 parentFilter = parentSprite.getFilter();
			parentFilter.a = 1.0f;
			parentSprite.setFilter(parentFilter);
			parentSprite.setTexture("Resources\\Assets\\ElaIdle.png");

			glm::vec4 colourFilter = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).getFilter();
			colourFilter.a = 0.0f;
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setFilter(colourFilter);
		}
	}
}