/* Start Header ************************************************************************/
/*!
\file           PatrolAI.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PatrolAI

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CoralElementBubble.h"
#include "ParentEvent.h"

namespace Rogue
{
	CoralElementBubble::CoralElementBubble(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_currentPointIndex{0}
	{
		LogicInit();
	}

	void CoralElementBubble::LogicInit()
	{
		if (g_engine.m_coordinator.ComponentExists<StatsComponent>(m_entity))
		{
			StatsComponent& stats = g_engine.m_coordinator.GetComponent<StatsComponent>(m_entity);
			for (auto& waypoint : stats.getWaypoints())
			{
				m_waypoints.push_back(waypoint);
			}
			if (stats.getWaypoints().size())
			{
				if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
				{
					trans->get().setPosition(*stats.getWaypoints().begin());
				}

				if (stats.getWaypoints().size() > 1)
				{
					m_nextPoint.push(*(stats.getWaypoints().begin() + 1));
				}
				else
				{
					m_nextPoint.push(*stats.getWaypoints().begin());
				}
			}
		}

		if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity))
		{
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).setIsStatic(true);
		}

		m_patrolDelay = 2.0f;
		m_delay = 0.0f;
	}

	void CoralElementBubble::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Patrol));
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void CoralElementBubble::AIPatrolUpdate()
	{
		//Only can do waypoint patrol if 2 waypoints exist
		//if (m_waypoints.size() < 2)
			//return;
		//Only can do waypoint patrol if set to Patrolling
		if (!m_statsComponent->GetIsPatrolling())
			return;

		if (m_delay > 0.0f)
		{
			m_delay -= g_engine.GetTimeScale() * g_deltaTime;
			if (m_delay < 0.0f)
				m_delay = 0.0f;
			return;
		}

		//Check if Transform component and Rigidbody exist
		if (!(g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity) && 
			g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity) &&
			m_statsComponent != nullptr))
			return;

		ParentTransformEvent event(m_entity, true);
		event.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
		EventDispatcher::instance().AddEvent(event);

		TransformComponent& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);

		//Always move
		Vec2 travelDistance, travelDistValue;
		
		if (m_nextPoint.size())
			travelDistValue = m_nextPoint.front() - aiTransform.GetPosition();
		else if (m_waypoints.size())
		{
			if (m_waypoints.size() == 1)
				return;

			m_nextPoint.push(m_waypoints.front());
			travelDistValue = m_nextPoint.front() - aiTransform.GetPosition();
		}
		else
			return;

		Vec2Normalize(travelDistance, travelDistValue);
		aiTransform.setPosition(aiTransform.GetPosition() + travelDistance * m_statsComponent->getSpeed() * DT_TRANSFORM_MODIFIER);

		for (auto& child : g_engine.m_coordinator.GetHierarchyInfo(m_entity).m_children)
		{
			g_engine.m_coordinator.GetComponent<ChildComponent>(child).SetGlobalDirty();
		}

		//If within a certain radius, assign next point
		if (Vec2SqDistance(aiTransform.GetPosition(), m_nextPoint.front()) < m_statsComponent->getSightRange() * m_statsComponent->getSightRange())
		{
			m_nextPoint.pop();

			if (m_waypoints.size() == 1)
				return;

			if (++m_currentPointIndex >= m_waypoints.size())
				m_currentPointIndex = 0;

			m_nextPoint.push(m_waypoints[m_currentPointIndex]);

			m_delay = m_patrolDelay;
		}
	}
}