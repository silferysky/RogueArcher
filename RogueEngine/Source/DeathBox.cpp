#include "Precompiled.h"
#include "DeathBox.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator
#include "EventDispatcher.h"
#include "GraphicsEvent.h"

namespace Rogue
{
	DeathBox::DeathBox(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent) :
		ScriptComponent(entity, logicComponent, statsComponent),
		m_timer{ 0.0f }
	{}

	void DeathBox::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void DeathBox::AIIdleUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive() || PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES || !PLAYER_STATUS.GetDeath())
			return;

		m_timer += g_deltaTime * g_engine.GetTimeScale();

		if (m_timer >= 0.4592166f && m_timer < 2.0f)
		{
			if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				transform->get().setZ(-9999); // Prefab player's Z value.
			}
			if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES)
			{
				PLAYER_STATUS.SetIndicatorStatus(false);
				g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetIndicator());
				g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetHitchhikeIndicator());
				PLAYER_STATUS.SetIndicator(MAX_ENTITIES);
				PLAYER_STATUS.SetHitchhikeIndicator(MAX_ENTITIES);
			}
			return;
		}
		
		if (m_timer >= 2.0f)
		{
			PLAYER_STATUS.SetDeath(false);
			PLAYER_STATUS.UnfreezeControls();
			m_timer = 0.0f;

			if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				transform->get().setPosition(PLAYER_STATUS.GetCheckpoint());
				transform->get().setZ(101); // Prefab player's Z value.
			}

			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);

			PLAYER_STATUS.SetIndicatorStatus(true);
		}
	}

	void DeathBox::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (other == PLAYER_STATUS.GetPlayerEntity())
		{
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
			CameraShakeEvent shake(5.0f);
			shake.SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
			EventDispatcher::instance().AddEvent(shake);

			PLAYER_STATUS.FreezeControls();
			PLAYER_STATUS.SetDeath(true);
			g_engine.m_coordinator.loadSound("Resources/Sounds/die3.ogg", 0.3f, false).Play();

			m_timer = 0.0f;
			
			if (other != MAX_ENTITIES)
			{
				TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity());

				trans.setZ(-9999);
			}
		}
	}
}