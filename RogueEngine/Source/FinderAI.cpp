/* Start Header ************************************************************************/
/*!
\file           FinderAI.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for FinderAI

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "FinderAI.h"

namespace Rogue
{
	FinderAI::FinderAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent) {}

	void FinderAI::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

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
			//Skip if found self
			if (iterator == m_entity)
				continue;

			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(iterator))
			{
				if (g_engine.m_coordinator.ComponentExists<TransformComponent>(iterator) && g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity))
				{
					auto& playerTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(iterator);
					auto& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);

					float distance = Vec2SqDistance(aiTransform.GetPosition(), playerTransform.GetPosition());

					//Empty this queue
					std::queue<Vec2> empty;
					std::swap(empty, m_nextPoint);

					if (distance < DEF_DETECT_RANGE * DEF_DETECT_RANGE)
					{
						////RE_CORE_INFO("IN RANGE");
						m_nextPoint.push(playerTransform.GetPosition());
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
		{
			return;
		}

		//Check if Transform component and Rigidbody exist
		if (!(g_engine.m_coordinator.ComponentExists<TransformComponent>(m_entity) &&
			g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_entity) && 
			m_statsComponent != nullptr))
			return;

		TransformComponent& aiTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity);

		if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(m_entity))
		{
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsLooping(true);
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsAnimating(true);
		}

		float distance = Vec2SqDistance(aiTransform.GetPosition(), m_nextPoint.front());

		//if (distance < m_statsComponent->getSpeed() * DT_SPEED_MODIFIER)
			//aiTransform.setPosition(m_nextPoint.front());
		//else
		{
			Vec2 travelDistance;
			Vec2Normalize(travelDistance, m_nextPoint.front() - aiTransform.GetPosition());
			RigidbodyComponent rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entity);
			//rigidbody.addForce(travelDistance * m_statsComponent->getSpeed() * DT_SPEED_MODIFIER);
			aiTransform.setPosition(aiTransform.GetPosition() + travelDistance * m_statsComponent->getSpeed() * DT_TRANSFORM_MODIFIER);
			
			//If facing right and moving left or facing left and moving right, flip
			if ((travelDistance.x < 0 && aiTransform.GetScale().x > 0.0f) || 
				(travelDistance.x > 0 && aiTransform.GetScale().x < 0.0f))
				aiTransform.setScale(Vec2(-1 * aiTransform.GetScale().x, aiTransform.GetScale().y));

			//If facing up and moving down or facing down and moving up, flip
			if (((travelDistance.y < 0 && aiTransform.GetScale().y > 0.0f) ||
				(travelDistance.y > 0 && aiTransform.GetScale().y < 0.0f)) && !rigidbody.getIsStatic())
				aiTransform.setScale(Vec2(aiTransform.GetScale().x, -1 * aiTransform.GetScale().y));
		
				//transform.setScale(1 * g_engine.m_coordinator.GetComponent<TransformComponent>(m_entity).getScale());
		}
	}

	void FinderAI::AIIdleUpdate()
	{
		if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(m_entity))
		{
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsLooping(false);
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsAnimating(false);
		}
	}
}