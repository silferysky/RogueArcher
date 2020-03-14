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
		m_age{ 0.0f }, m_lifetime{ 3.0f }, m_triggered{ false }, m_playerZ{ 100 }
	{}

	void DeathBox::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void DeathBox::AIIdleUpdate()
	{
		if (!m_triggered || !g_engine.m_coordinator.GameIsActive() || PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES)
			return;

		g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);

		// Once death timer has reached lifetime
		if (m_age > m_lifetime)
		{
			// Enable player statuses
			PLAYER_STATUS.SetDeath(false);
			PLAYER_STATUS.UnfreezeControls();
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);
			m_triggered = false;
			
			if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				transform->get().setPosition(PLAYER_STATUS.GetCheckpoint());
				transform->get().setZ(104); // Prefab player's Z value.
			}

			// Reenable camera lerping after shake
			//g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);

			// Respawn indicators
			PLAYER_STATUS.SetIndicatorStatus(true);
		}

		// Increment age by deltatime
		m_age += g_deltaTime * g_engine.GetTimeScale();
	}

	void DeathBox::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		// If trigger hits player
		if (other == PLAYER_STATUS.GetPlayerEntity())
		{
			// Shake camera
			CameraShakeEvent shake(5.0f);
			shake.SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
			EventDispatcher::instance().AddEvent(shake);
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);

			// Player statuses
			PLAYER_STATUS.FreezeControls();
			PLAYER_STATUS.SetDeath(true);
			m_triggered = true;
			PLAYER_STATUS.DestroyIndicators(); // Indicators will spawn in PlayerControllerSystem update

			// Play death sound
			g_engine.m_coordinator.loadSound("Resources/Sounds/die3.ogg", 0.3f, false).Play();

			// Start timer and set lifetime
			m_age = 0.0f;
			m_lifetime = 2.5f;

			// Set player and children to be unseen
			TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity());
			m_playerZ = trans.GetZ();
			trans.setZ(-9999);
		}
	}
}