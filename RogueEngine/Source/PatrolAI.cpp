#include "Precompiled.h"
#include "PatrolAI.h"
#include "Main.h"

namespace Rogue
{
	PatrolAI::PatrolAI(Entity entity, LogicComponent& logicComponent)
		: BaseAI(entity, logicComponent), m_currentPointIndex{0} 
	{
		//AddWaypoint(g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition() - Vec2(100, 0));
		//AddWaypoint(g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition());
		//m_nextPoint.push(m_waypoints[0]);
		LogicInit();
	}

	void PatrolAI::LogicInit()
	{
		if (g_engine.m_coordinator.ComponentExists<StatsComponent>(m_entity))
		{
			StatsComponent& stats = g_engine.m_coordinator.GetComponent<StatsComponent>(m_entity);
			for (auto& waypoint : stats.getWaypoints())
			{
				m_waypoints.push_back(waypoint);
			}
		}

		if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity))
		{
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).setIsStatic(true);
		}
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
		if (!(g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity) && 
			g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity) &&
			m_statsComponent != nullptr))
			return;

		TransformComponent& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);

		//Always move
		Vec2 travelDistance, travelDistValue;
		
		if (m_nextPoint.size())
			travelDistValue = m_nextPoint.front() - aiTransform.GetPosition();
		else if (m_waypoints.size())
		{
			m_nextPoint.push(m_waypoints.front());
			travelDistValue = m_nextPoint.front() - aiTransform.GetPosition();
		}
		else
			return;

		Vec2Normalize(travelDistance, travelDistValue);
		aiTransform.setPosition(aiTransform.GetPosition() + travelDistance * m_statsComponent->getSpeed() * DT_TRANSFORM_MODIFIER);

		//If within a certain radius, assign next point
		if (Vec2SqDistance(aiTransform.GetPosition(), m_nextPoint.front()) < m_statsComponent->getSightRange() * m_statsComponent->getSightRange())
		{
			m_nextPoint.pop();
			if (++m_currentPointIndex >= m_waypoints.size())
				m_currentPointIndex = 0;
			m_nextPoint.push(m_waypoints[m_currentPointIndex]);

			//If facing right and moving left or facing left and moving right, flip
			if ((travelDistance.x < 0 && aiTransform.GetScale().x > 0.0f) || 
				(travelDistance.x > 0 && aiTransform.GetScale().x < 0.0f))
				aiTransform.setScale(Vec2(-1 * aiTransform.GetScale().x, aiTransform.GetScale().y));

			//If facing up and moving down or facing down and moving up, flip
			if ((travelDistance.y < 0 && aiTransform.GetScale().y > 0.0f) ||
				(travelDistance.y > 0 && aiTransform.GetScale().y < 0.0f))
				aiTransform.setScale(Vec2(aiTransform.GetScale().x, -1 * aiTransform.GetScale().y));
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