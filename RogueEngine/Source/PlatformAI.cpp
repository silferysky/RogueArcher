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
	}

	void PlatformAI::AIPatrolUpdate()
	{
		PatrolAI::AIPatrolUpdate();

		//Since new waypoint just selected, it means now it is going opposite way
		Vec2 firstPos = *m_waypoints.begin();
		Vec2 secondPos = *(m_waypoints.begin() + 1);

		//If m_delay == m_patrolDelay, it means a new waypoint is just selected
		if (m_delay == m_patrolDelay)
		{
			HierarchyInfo& info = g_engine.m_coordinator.GetHierarchyInfo(m_entity);

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
			if (m_currentPointIndex)
			{
				ev = new EntChangeSpriteEvent(toChangeSprite, "Resources\\Assets\\FlowerPlatformOpen.png");
			}
			else
			{
				ev = new EntChangeSpriteEvent(toChangeSprite, "Resources\\Assets\\FlowerPlatformClose.png");
			}
			ev->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);
		}
		else if (m_delay > 0.0f)
		{

		}

	}
}