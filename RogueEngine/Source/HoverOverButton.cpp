/* Start Header ************************************************************************/
/*!
\file           HoverOverButton.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for HoverOverButton

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "HoverOverButton.h"
#include "CollisionManager.h"
#include "PickingManager.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	HoverOverButton::HoverOverButton(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
		if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(entity))
		{
			m_originalScale = transform->get().GetScale();
		}
	}

	void HoverOverButton::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void HoverOverButton::AIIdleUpdate()
	{
		if (auto transform = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
		{
			auto& transformOpt = transform->get();
			PickingManager::instance().GenerateMeshAABB(transformOpt);

			// cursor is on the button
			if (CollisionManager::instance().DiscretePointVsAABB(PickingManager::instance().GetWorldCursor(), transformOpt.GetPickArea()))
			{
				transformOpt.setScale(m_originalScale * 1.15f);
			}
			else
			{
				transformOpt.setScale(m_originalScale);
			}
		}
	}
}