#include "Precompiled.h"
#include "FinderAI.h"
#include "Main.h"

namespace Rogue
{
	FinderAI::FinderAI(Entity entity, LogicComponent& logicComponent)
		: BaseAI(entity, logicComponent) {}

	void FinderAI::AIActiveStateUpdate()
	{
		AIDetect();
		if (!m_nextPoint.empty())
			m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Chase));
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void FinderAI::AIDetect()
	{
		auto& activeObjects = g_engine.m_coordinator.GetActiveObjects(); 
		
		for (auto& iterator : activeObjects)
		{
			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(iterator.m_Entity))
			{
				if (g_engine.m_coordinator.ComponentExists<TransformComponent>(iterator.m_Entity) && g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity))
				{
					auto& playerTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(iterator.m_Entity);
					auto& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);

					float distance = Vec2SqDistance(aiTransform.getPosition(), playerTransform.getPosition());

					//Empty this queue
					std::queue<Vec2> empty;
					std::swap(empty, m_nextPoint);

					if (distance < DEF_DETECT_RANGE * DEF_DETECT_RANGE)
					{
						//RE_CORE_INFO("IN RANGE");
						m_nextPoint.push(playerTransform.getPosition());
					}
				}
				break;
			}
		}
	}

	void FinderAI::AIChaseUpdate()
	{
		//Check if next point exists
		if (m_nextPoint.empty())
			return;

		//Check if Transform component and Rigidbody exist
		if (!(g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity) &&
			g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity) && 
			g_engine.m_coordinator.ComponentExists<StatsComponent>(m_entity)))
			return;

		TransformComponent& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);
		StatsComponent& aiStats = g_engine.m_coordinator.GetComponent<StatsComponent>(m_entity);

		float distance = Vec2SqDistance(aiTransform.getPosition(), m_nextPoint.front());

		if (distance < aiStats.getSpeed() * DT_SPEED_MODIFIER)
			aiTransform.setPosition(m_nextPoint.front());
		else
		{
			Vec2 travelDistance;
			Vec2Normalize(travelDistance, m_nextPoint.front() - aiTransform.getPosition());
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).addForce(travelDistance * aiStats.getSpeed() * DT_SPEED_MODIFIER);
			
			//If facing right and moving left or facing left and moving right, flip
			if ((travelDistance.x < 0 && aiTransform.getScale().x > 0.0f) || 
				(travelDistance.x > 0 && aiTransform.getScale().x < 0.0f))
				aiTransform.setScale(Vec2(-1 * aiTransform.getScale().x, aiTransform.getScale().y));

			//If facing up and moving down or facing down and moving up, flip
			if ((travelDistance.y < 0 && aiTransform.getScale().y > 0.0f) ||
				(travelDistance.y > 0 && aiTransform.getScale().y < 0.0f))
				aiTransform.setScale(Vec2(aiTransform.getScale().x, -1 * aiTransform.getScale().y));
		
				//transform.setScale(1 * g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity).getScale());
		}
	}
}