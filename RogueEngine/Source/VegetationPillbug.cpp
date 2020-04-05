/* Start Header ************************************************************************/
/*!
\file           VegetationPillbug.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for VegetationPillbug

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "VegetationPillbug.h"
#include "PlayerStatusManager.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	VegetationPillbug::VegetationPillbug(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void VegetationPillbug::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (m_entity == PLAYER_STATUS.GetHitchhikedEntity()) // if this entity has been hitchhiked onto
		{
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).setIsStatic(false); // switch off the rigidbody
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setTexturePath("Resources/Assets/VegetationPillbugCurl.png");

			auto& animation = g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity);
			animation.setFrames(5);
			animation.setCurrentFrame(0);
			animation.setEndFrame(4);
			animation.setIsLooping(false); // play the animation only once

			PLAYER_STATUS.FreezeControls();

			m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
		}
	}

	/* void VegetationPillbug::AIIdleUpdate()
	{
	} */


	void VegetationPillbug::OnCollisionEnter(Entity other)
	{
		if (g_engine.m_coordinator.GetHierarchyInfo(other).m_tag == "Ground")
		{
			if (m_entity == PLAYER_STATUS.GetHitchhikedEntity()) // if player is still attached
				g_engine.m_coordinator.GetSystem<PlayerControllerSystem>()->Hitchhike(MAX_ENTITIES); // Proper way to get out of hitchhike

			PLAYER_STATUS.UnfreezeControls();

			g_engine.m_coordinator.AddToDeleteQueue(m_entity);
		}
	}
}