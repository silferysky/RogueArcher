#include "Precompiled.h"
#include "DeathBox.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator
#include "EventDispatcher.h"
#include "GraphicsEvent.h"

namespace Rogue
{
	DeathBox::DeathBox(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void DeathBox::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void DeathBox::AIIdleUpdate()
	{
		if (PlayerStatusManager::instance().GetDeath())
		{
			m_timer += g_deltaTime * g_engine.GetTimeScale();
			if (m_timer >= 0.4592166f && m_timer < 3.0f)
			{
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
				if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_other))
					transform->get().setPosition(PlayerStatusManager::instance().GetCheckpoint());
				
				if (PLAYER_STATUS.GetPlayerEntity() != MAX_ENTITIES && g_engine.m_coordinator.ComponentExists<SpriteComponent>(PlayerStatusManager::instance().GetPlayerEntity()))
				{
					if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_other))
						sprite->get().setFilter(glm::vec4(sprite->get().getFilter().r, sprite->get().getFilter().g, sprite->get().getFilter().b, 0.0f));
				}

				if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES)
				{
					PLAYER_STATUS.SetIndicatorStatus(false);
					g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetIndicator());
					g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetHitchhikeIndicator());
					PLAYER_STATUS.SetIndicator(MAX_ENTITIES);
					PLAYER_STATUS.SetHitchhikeIndicator(MAX_ENTITIES);
				}
			}
			else if (m_timer >= 3.0f)
			{
				m_timer = 0.0f;
				PlayerStatusManager::instance().SetDeath(false);
				if (auto player = g_engine.m_coordinator.TryGetComponent<PlayerControllerComponent>(m_other))
					player->get().SetIsActive(true);
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);

				if (PLAYER_STATUS.GetPlayerEntity() != MAX_ENTITIES)
				{
					if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(PLAYER_STATUS.GetPlayerEntity()))
						sprite->get().setFilter(glm::vec4(sprite->get().getFilter().r, sprite->get().getFilter().g, sprite->get().getFilter().b, 1.0f));
				}

				PLAYER_STATUS.SetIndicatorStatus(true);
			}
		}
	}

	void DeathBox::OnTriggerEnter(Entity other)
	{
		if (auto player = g_engine.m_coordinator.TryGetComponent<PlayerControllerComponent>(other))
		{
			//g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
			CameraShakeEvent shake(20.0f);
			shake.SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
			EventDispatcher::instance().AddEvent(shake);

			PlayerStatusManager::instance().SetDeath(true);
			g_engine.m_coordinator.loadSound("Resources/Sounds/die3.ogg", 0.3f, false).Play();
			m_other = other;
			player->get().SetIsActive(false);
		}
	}
}