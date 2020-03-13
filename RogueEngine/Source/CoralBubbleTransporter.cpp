#include "Precompiled.h"
#include "CoralBubbleTransporter.h"
#include "PlayerStatusManager.h"
#include "ParentEvent.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	CoralBubbleTransporter::CoralBubbleTransporter(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_currentPointIndex{ 0 }
	{
		LogicInit();
	}

	void CoralBubbleTransporter::LogicInit()
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
		m_statsComponent->SetIsPatrolling(false);
	}

	void CoralBubbleTransporter::AIPatrolUpdate()
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
			//If only 1 waypoint, no need to pop and replace
			if (m_waypoints.size() == 1)
				return;

			m_nextPoint.pop();

			//At the very last waypoint
			if (++m_currentPointIndex >= m_waypoints.size())
			{
				g_engine.m_coordinator.GetSystem<PlayerControllerSystem>()->Hitchhike(MAX_ENTITIES);
				//g_engine.m_coordinator.AddToDeleteQueue(m_entity);
				m_statsComponent->SetIsPatrolling(false);
				m_currentPointIndex = 1;
				
				if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(m_entity))
				{
					trans->get().setPosition(m_waypoints[0]);
				}
			}

			m_nextPoint.push(m_waypoints[m_currentPointIndex]);

			m_delay = m_patrolDelay;
		}
	}

	void CoralBubbleTransporter::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (m_statsComponent->GetIsPatrolling())
			m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Patrol));

		//m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void CoralBubbleTransporter::AIIdleUpdate()
	{
		
	}

	void CoralBubbleTransporter::OnTriggerEnter(Entity other)
	{
		if (other == PLAYER_STATUS.GetPlayerEntity())
		{
			g_engine.m_coordinator.GetSystem<PlayerControllerSystem>()->Hitchhike(m_entity); // force hitchhike
			m_statsComponent->SetIsPatrolling(true);
		}
	}
}