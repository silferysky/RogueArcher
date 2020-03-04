#include "Precompiled.h"
#include "HoverOverButton.h"
#include "CollisionManager.h"
#include "PickingManager.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	HoverOverButton::HoverOverButton(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void HoverOverButton::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void HoverOverButton::AIIdleUpdate()
	{
		auto& pickArea = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity).GetPickArea();

		// cursor is on the button
		if (CollisionManager::instance().DiscretePointVsAABB(PickingManager::instance().GetWorldCursor(), pickArea))
		{
			// put the hover logic here: stuff like play a sound, change the button colour etc...
			auto sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity);

			sprite.setFilter(glm::vec4(1, 0, 0, 0));
		}
	}
}