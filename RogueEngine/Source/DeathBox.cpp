#include "Precompiled.h"
#include "DeathBox.h"
#include "PlayerStatusManager.h"
#include "Main.h"	//For g_deltaTime and coordinator
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
			if (m_timer >= 0.65f && m_timer < 3.0f)
			{
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
				g_engine.m_coordinator.GetComponent<TransformComponent>(m_other).setPosition(PlayerStatusManager::instance().GetCheckpoint());
				g_engine.m_coordinator.loadSound("Resources/Sounds/soul_pickup.ogg", 0.3f, false).Play();
				
				if (PLAYER_STATUS.GetPlayerEntity() != MAX_ENTITIES && g_engine.m_coordinator.ComponentExists<SpriteComponent>(PlayerStatusManager::instance().GetPlayerEntity()))
				{
					SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_other);
					sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, 0.0f));
				}

				if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES)
				{
					PLAYER_STATUS.SetIndicatorStatus(false);
					g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetIndicator());
					PLAYER_STATUS.SetIndicator(MAX_ENTITIES);
				}
			}
			else if (m_timer >= 3.0f)
			{
				m_timer = 0.0f;
				PlayerStatusManager::instance().SetDeath(false);
				g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(m_other).SetIsActive(true);
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);

				if (PLAYER_STATUS.GetPlayerEntity() != MAX_ENTITIES && g_engine.m_coordinator.ComponentExists<SpriteComponent>(PlayerStatusManager::instance().GetPlayerEntity()))
				{
					SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(PLAYER_STATUS.GetPlayerEntity());
					sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, 1.0f));
				}

				PLAYER_STATUS.SetIndicatorStatus(true);
			}
		}
	}

	void DeathBox::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			//g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
			CameraShakeEvent* shake = new CameraShakeEvent(20.0f);
			shake->SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
			EventDispatcher::instance().AddEvent(shake);

			PlayerStatusManager::instance().SetDeath(true);
			m_other = other;
			g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(m_other).SetIsActive(false);
		}
	}
}