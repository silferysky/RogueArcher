#include "Precompiled.h"
#include "VegetationBreakable.h"
#include "PlayerStatusManager.h"
#include "GraphicsEvent.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	VegetationBreakable::VegetationBreakable(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
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
		}
	}
}
