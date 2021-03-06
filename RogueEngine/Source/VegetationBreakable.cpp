/* Start Header ************************************************************************/
/*!
\file           VegetationBreakable.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for VegetationBreakable

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "VegetationBreakable.h"
#include "PlayerStatusManager.h"
#include "GraphicsEvent.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	VegetationBreakable::VegetationBreakable(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_triggered{false}
	{
	}

	void VegetationBreakable::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void VegetationBreakable::AIIdleUpdate()
	{
		if (!m_triggered || !g_engine.m_coordinator.GameIsActive() || PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES)
			return;

		// Once death timer has reached lifetime
		if (m_timer > 2.0f)
		{
			PLAYER_STATUS.UnfreezeControls();
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);
			m_triggered = false;
		}

		// Increment age by deltatime
		m_timer += g_deltaTime * g_engine.GetTimeScale();
	}

	void VegetationBreakable::OnTriggerEnter(Entity other)
	{
		if (other == PLAYER_STATUS.GetHitchhikedEntity()) // if collided into by the hitchhiked entity
		{
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).setIsStatic(false); // switch off the rigidbody
			g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(m_entity).SetCollisionMode(CollisionMode::e_asleep);

			CameraShakeEvent shake(5.0f);
			shake.SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
			EventDispatcher::instance().AddEvent(shake);
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
			m_triggered = true;

			g_engine.m_coordinator.loadSound("Resources/Sounds/woodbreak.ogg").Play();
		}
	}
}
