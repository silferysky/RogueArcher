#include "PatrolAI.h"
#include "Main.h"

namespace Rogue
{
	PatrolAI::PatrolAI(Entity entity, LogicComponent& logicComponent)
		: BaseAI(entity, logicComponent), m_currentPointIndex{0} 
	{
		AddWaypoint(g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition());
		AddWaypoint(g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition() - Vec2(100,0));
		m_nextPoint.push(m_waypoints[0]);
	}

	void PatrolAI::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Patrol));
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void PatrolAI::AIPatrolUpdate()
	{
		//Only can do waypoint patrol if 2 waypoints exist
		if (m_waypoints.size() < 2)
			return;

		//Check if Transform component and Rigidbody exist
		if (!(g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity) && g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity)))
			return;

		TransformComponent& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);

		//Always move
		Vec2 travelDistance;
		Vec2Normalize(travelDistance, m_nextPoint.front() - aiTransform.getPosition());
		aiTransform.setPosition(aiTransform.getPosition() + travelDistance * SPEED);

		//If within a certain radius, assign next point
		if (Vec2SqDistance(aiTransform.getPosition(), m_nextPoint.front()) < PATROL_RANGE * PATROL_RANGE)
		{
			m_nextPoint.pop();
			if (++m_currentPointIndex >= m_waypoints.size())
				m_currentPointIndex = 0;
			m_nextPoint.push(m_waypoints[m_currentPointIndex]);
		}

	}

	void PatrolAI::AddWaypoint(Vec2 newPoint)
	{
		m_waypoints.push_back(newPoint);
	}

	void PatrolAI::ClearWaypoints()
	{
		m_waypoints.clear();
	}

	std::vector<Vec2> PatrolAI::GetWaypoints()
	{
		return m_waypoints;
	}
}