#include "Precompiled.h"
#include "TeleportVFXHandler.h"

#define VFX_PER_TELEPORT 3
#define VFX_MOVESPEED 4.0f
#define VFX_MAX_DIST 5.0f

namespace Rogue
{
	TeleportVFXHandler::TeleportVFXHandler(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_teleportVFXArray{ std::vector<Entity>() }
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
		for (auto entIt = std::begin(m_teleportVFXArray); entIt != std::end(m_teleportVFXArray); ++entIt)
		{
			if (auto entTrans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(*entIt))
			{
				Vec2 diff = transform.GetPosition() - entTrans->get().GetPosition();

				if (Vec2SqLength(diff) > VFX_MAX_DIST* VFX_MAX_DIST)
				{
					entTrans->get().setPosition(entTrans->get().GetPosition() + diff * g_deltaTime * VFX_MOVESPEED);

					Vec2 entScale = entTrans->get().GetScale();
	
					//Checking for direction

					//If facing right and need to turn left
					if (diff.x < 0.0f && entScale.x > 0.0f)
					{
						entScale.x *= -1.0f;
						entTrans->get().setScale(entScale);
					}
					//If facing left and need to turn right
					else if (diff.x > 0.0f && entScale.x < 0.0f)
					{
						entScale.x *= -1.0f;
						entTrans->get().setScale(entScale);
					}
				}
				else
				{
					g_engine.m_coordinator.AddToDeleteQueue(*entIt);
					auto tempIt = entIt--;
					PLAYER_STATUS.IncrementTeleportCount(-1);
					m_teleportVFXArray.erase(tempIt);
				}
			}
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