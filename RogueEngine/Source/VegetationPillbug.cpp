#include "Precompiled.h"
#include "VegetationBreakable.h"
#include "PlayerStatusManager.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	VegetationBreakable::VegetationBreakable(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void VegetationBreakable::AIActiveStateUpdate()
	{
		if (m_entity == PLAYER_STATUS.GetHitchhikedEntity()) // if this entity has been hitchhiked onto
		{
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).setIsStatic(false); // switch off the rigidbody
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsAnimating(true); // play the animation
			m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
		}
	}

	void VegetationBreakable::AIIdleUpdate()
	{

	}
}