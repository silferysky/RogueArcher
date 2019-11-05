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
		auto iterator = activeObjects.begin();
		for (; iterator != activeObjects.end(); ++iterator)
		{
			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(iterator->m_Entity))
			{
				if (g_engine.m_coordinator.ComponentExists<TransformComponent>(iterator->m_Entity) && g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity))
				{
					auto& playerTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(iterator->m_Entity);
					auto& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);

					float distance = Vec2SqDistance(aiTransform.getPosition(), playerTransform.getPosition());

					//Empty this queue
					std::queue<Vec2> empty;
					std::swap(empty, m_nextPoint);

					if (distance < RANGE * RANGE)
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
		if (!(g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity) && g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity)))
			return;

		TransformComponent& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);
		float distance = Vec2SqDistance(aiTransform.getPosition(), m_nextPoint.front());

		if (distance < SPEED)
			aiTransform.setPosition(m_nextPoint.front());
		else
		{
			Vec2 travelDistance;
			Vec2Normalize(travelDistance, m_nextPoint.front() - aiTransform.getPosition());
			g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity).addForce(travelDistance * SPEED);
		}
	}
}