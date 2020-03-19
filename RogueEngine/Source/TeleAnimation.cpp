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

					// make animation visible
					glm::vec4 colourFilter = sprite->get().getFilter();
					colourFilter.a = 1.0f;
					sprite->get().setFilter(colourFilter);

					// make player transparent
					TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity());
					trans.setZ(-9999);

					// freeze camera
					g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
					m_timerActive = true;
					PLAYER_STATUS.SetIsTeleporting(true);
					PLAYER_STATUS.FreezeControls();

					/*if (auto child = g_engine.m_coordinator.TryGetComponent<ChildComponent>(m_entity))
					{
						child->get().SetIsFollowing(false);
						if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
							if (auto parentTransform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(child->get().GetParent()))
								transform->get().setPosition(parentTransform->get().GetPosition());
					} */

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

		if (m_timer > 0.2f)
		{
			m_timerActive = false;
			m_timer = 0.0f;
			PLAYER_STATUS.SetIsTeleporting(false);
			PLAYER_STATUS.UnfreezeControls();

			// unfreeze camera
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);

			// make player reappear
			TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity());
			trans.setZ(104);
		}

		m_teleCharge = PlayerStatusManager::instance().GetTeleportCharge();

		if (m_timerActive)
			m_timer += g_deltaTime * g_engine.GetTimeScale();
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