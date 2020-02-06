#include "Precompiled.h"
#include "PlatformAI.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "GameEvent.h"

namespace Rogue
{
	PlatformAI::PlatformAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: PatrolAI(entity, logicComponent, statsComponent)
	{
		//HierarchyInfo& info = g_engine.m_coordinator.GetHierarchyInfo(entity);
		//for (auto child : info.m_children)
		//{
		//	if (g_engine.m_coordinator.ComponentExists<ChildComponent>(child))
		//	{
		//		g_engine.m_coordinator.GetComponent<ChildComponent>(child).SetIsFollowing(false);
		//	}
		//}
	}

	void PlatformAI::AIPatrolUpdate()
	{
		if (PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES)
			return;

		PatrolAI::AIPatrolUpdate();

		//Since new waypoint just selected, it means now it is going opposite way
		Vec2 firstPos = *m_waypoints.begin();
		Vec2 secondPos = *(m_waypoints.begin() + 1);

		HierarchyInfo& info = g_engine.m_coordinator.GetHierarchyInfo(m_entity);
		if (info.m_children.size())
		{

		}

		//If m_delay == m_patrolDelay, it means a new waypoint is just selected
		if (m_delay == m_patrolDelay)
		{
			if (!info.m_children.size())
				return;

			Entity toChangeSprite = MAX_ENTITIES;

			for (auto child : info.m_children)
			{
				if (g_engine.m_coordinator.GetHierarchyInfo(child).m_tag == "Change")
				{
					toChangeSprite = child;
					break;
				}
			}

			if (toChangeSprite == MAX_ENTITIES)
				return;

			//Changing buds
			EntChangeSpriteEvent* ev;
			if ((m_currentPointIndex && firstPos.y > secondPos.y) || (!m_currentPointIndex && firstPos.y < secondPos.y))
			{
				ev = new EntChangeSpriteEvent(toChangeSprite, "Resources\\Assets\\FlowerPlatformOpen.png");
			}
			else
			{
				ev = new EntChangeSpriteEvent(toChangeSprite, "Resources\\Assets\\FlowerPlatformClose.png");
			}
			ev->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);

			//Manually preventing second occurance of this event
			m_delay -= 0.001f;
		}
		else if (m_delay == 0.0f)
		{
			EntChangeSpriteEvent* ev;
			if ((m_currentPointIndex && firstPos.y > secondPos.y) || (!m_currentPointIndex && firstPos.y < secondPos.y))
			{
				ev = new EntChangeSpriteEvent(m_entity, "Resources\\Assets\\FlowerDown.png");

				if (g_engine.m_coordinator.ComponentExists<ColliderComponent>(m_entity))
				{
					ColliderComponent& collider = g_engine.m_coordinator.GetComponent<ColliderComponent>(m_entity);
					int lightPos = -1;
					int darkPos = -1;

					lightPos = LayerManager::instance().GetLayerCategory("Light");
					darkPos = LayerManager::instance().GetLayerCategory("Dark");

					collider.ChangeLayer(darkPos);
					collider.SetMask(lightPos);
					collider.SetMask(darkPos, false);
				}
			}
			else
			{
				ev = new EntChangeSpriteEvent(m_entity, "Resources\\Assets\\FlowerUp.png");

				if (g_engine.m_coordinator.ComponentExists<ColliderComponent>(m_entity))
				{
					ColliderComponent& collider = g_engine.m_coordinator.GetComponent<ColliderComponent>(m_entity);
					int lightPos = -1;
					int darkPos = -1;

					lightPos = LayerManager::instance().GetLayerCategory("Light");
					darkPos = LayerManager::instance().GetLayerCategory("Dark");

					collider.ChangeLayer(lightPos);
					collider.SetMask(darkPos);
					collider.SetMask(lightPos, false);
				}
			}
			ev->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);

			//Manually preventing second occurance of this event
			m_delay -= 0.001f;
		}

	}
}