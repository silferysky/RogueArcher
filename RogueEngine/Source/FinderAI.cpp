#include "FinderAI.h"
#include "Main.h"

namespace Rogue
{
	FinderAI::FinderAI(Entity entity, LogicComponent& logicComponent)
		: BaseAI(entity, logicComponent) {}

	void FinderAI::LogicInit()
	{
		//Put in order of importance
		//Use AddAIState for active states, AddAIStateInactive for inactive states that might turn active
		m_logicComponent->AddAIState(AIState::AIState_Chase);
		m_logicComponent->AddAIState(AIState::AIState_Idle);

		//Sets initial state of AI
		m_logicComponent->CurState(AIState::AIState_Idle);
	}

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

					if (distance < RANGE * RANGE)
					{
						RE_CORE_INFO("IN RANGE");
						std::queue<Vec2> empty;
						std::swap(empty, m_nextPoint);
						m_nextPoint.push(playerTransform.getPosition());
					}
				}
				break;
			}
		}

	}

	void FinderAI::AIChaseUpdate()
	{
		if (m_nextPoint.empty())
			return;

		auto& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);
		float distance = Vec2SqDistance(aiTransform.getPosition(), m_nextPoint.front());

		if (distance < SPEED)
			aiTransform.setPosition(m_nextPoint.front());
		else
		{
			Vec2 travelDistance = m_nextPoint.front() - aiTransform.getPosition();
			aiTransform.setPosition(aiTransform.getPosition() + travelDistance * 0.1f);
		}
	}
}