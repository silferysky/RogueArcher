#include "Precompiled.h"
#include "TeleportVFXHandler.h"

#define VFX_PER_TELEPORT 3
#define VFX_MOVESPEED 0.02f
#define VFX_MAX_DIST 10.0f

namespace Rogue
{
	TeleportVFXHandler::TeleportVFXHandler(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_teleportVFXArray{ std::vector<Entity>() }, m_timer{ 0.0f }, m_delay{ 0.3f }
	{
	}
	void TeleportVFXHandler::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void TeleportVFXHandler::AIIdleUpdate()
	{
		//Safety check in case Player has no start location
		if (!g_engine.m_coordinator.ComponentExists<TransformComponent>(PLAYER_STATUS.GetPlayerEntity()))
			return;

		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity());

		//For handling all current teleportVFX
		for (Entity ent : m_teleportVFXArray)
		{
			if (auto entTrans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(ent))
			{
				Vec2 diff = transform.GetPosition() - entTrans->get().GetPosition();

				if (Vec2SqLength(diff) > VFX_MAX_DIST* VFX_MAX_DIST)
					entTrans->get().setPosition(entTrans->get().GetPosition());
			}
		}
		
		if (m_teleportVFXArray.size())
			m_timer += g_engine.GetTimeScale() * g_deltaTime;
		if (m_timer > m_delay)
		{
			for (Entity ent : m_teleportVFXArray)
			{
				g_engine.m_coordinator.AddToDeleteQueue(ent);
			}
			m_teleportVFXArray.clear();
			PLAYER_STATUS.SetTeleportCount(0);
			m_timer = 0.0f;
		}

		//For creating more teleport VFX
		if (PLAYER_STATUS.GetTeleportCount() * VFX_PER_TELEPORT > m_teleportVFXArray.size())
		{
			Vec2 newLoc = PLAYER_STATUS.GetPreTeleportLoc();
			Vec2 displacement = (transform.GetPosition() - newLoc);

			for (size_t i = 0; i < VFX_PER_TELEPORT; ++i, newLoc += displacement/VFX_PER_TELEPORT)
			{
				Entity newVFX;
				if (PLAYER_STATUS.GetLightStatus())
					newVFX = g_engine.m_coordinator.CloneArchetypes("ExaTeleportVFX", true, false);
				else
					newVFX = g_engine.m_coordinator.CloneArchetypes("ElaTeleportVFX", true, false);

				if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(newVFX))
				{
					trans->get().setPosition(newLoc);
				}

				m_teleportVFXArray.push_back(newVFX);
			}
		}
	}
}