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

	void VegetationBreakable::OnCollisionEnter(Entity other)
	{
		if (other == PLAYER_STATUS.GetHitchhikedEntity()) // if collided into by the hitchhiked entity
		{
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).setIsStatic(false); // switch off the rigidbody
		}
	}
}
