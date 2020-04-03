/* Start Header ************************************************************************/
/*!
\file           Checkpoint.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for Checkpoint

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "Checkpoint.h"
#include "PlayerStatusManager.h"
////#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	Checkpoint::Checkpoint(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_collected{false}
	{
	}

	void Checkpoint::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
			{
				fade->get().setIsActive(true);
				fade->get().setIsFadingIn(false);
			}
			
			g_engine.m_coordinator.loadSound("Resources/Sounds/soul_pickup.ogg", 0.3f, false).Play();
			if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
				PlayerStatusManager::instance().SetCheckpoint(transform->get().GetPosition());

			m_collected = true;
		}
	}
}