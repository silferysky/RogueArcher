/* Start Header ************************************************************************/
/*!
\file           ScriptComponent.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ScriptComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "ScriptComponent.h"
#include "Logger.h"
#include "PlayerStatusManager.h"
#include "GameEvent.h"

namespace Rogue
{
	ScriptComponent::ScriptComponent(Entity entity, LogicComponent& logicComp, StatsComponent& statsComp)
		: m_entity{entity}, m_logicComponent{std::make_shared<LogicComponent>(logicComp)}, m_statsComponent{std::make_shared<StatsComponent>(statsComp)}
	{
		LogicInit();
	}

	void ScriptComponent::LogicInit()
	{
		//Put in order of importance
		//Use AddAIState for active states, AddAIStateInactive for inactive states that might turn active
		//m_logicComponent->AddAIState(AIState::AIState_Patrol);
		//m_logicComponent->AddAIState(AIState::AIState_Idle);

		//Sets initial state of AI
		//m_logicComponent->SetCurState(AIState::AIState_Idle);
	}

	void ScriptComponent::LogicUpdate()
	{
		//For all possible states ScriptComponent has
		//This for loop handles the order of importance of each state.
		auto allStatesVector = m_logicComponent->GetAllAIStates();
		for (auto& it : allStatesVector)
		{
			//If its ActiveStateBit matches, run that update
			if (m_logicComponent->GetActiveStateBit(static_cast<size_t>(it)))
			{
				switch (it)
				{
				case AIState::AIState_Chase:
					AIChaseUpdate();
					break;
				case AIState::AIState_Patrol:
					AIPatrolUpdate();
					break;
				default:
					//Idle is default state
					AIIdleUpdate();
					break;
				}

				//Sets current state and exit state since you aren't supposed to do multiple states
				m_logicComponent->SetCurState(it);
				//Break when done so other states would not be performed
				break;
			}
		}
	}

	void ScriptComponent::AIDetect()
	{
	}

	void ScriptComponent::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit((size_t)AIState::AIState_Idle);
	}

	void ScriptComponent::AIChaseUpdate()
	{
		//RE_INFO("CHASING PLAYER");
	}

	void ScriptComponent::AIPatrolUpdate()
	{
		//RE_INFO("AI PATROLLING");
	}

	void ScriptComponent::AIIdleUpdate()
	{
		//RE_INFO("AI IDLE");
	}

	void ScriptComponent::OnCollisionEnter(Entity otherEnt)
	{
		//RE_INFO("COLLISION ENTER");
	}

	// Function overloading for different colliders
	void ScriptComponent::HandleCollision(EntCollisionOrTrigger<BoxCollider2DComponent, BoxCollider2DComponent>& ev)
	{
		switch (ev.GetEventType())
		{
		case EventType::EvOnCollisionEnter:
		{
			//Set entity to either first or second ID depending on which it isn't
			Entity otherEnt = ev.GetThis().m_entity;

			if (ev.GetThis().m_entity == m_entity)
				otherEnt = ev.GetOther().m_entity;

			if (ev.GetEventCat() & EventCatTrigger)
				OnTriggerEnter(otherEnt);
			else
				OnCollisionEnter(otherEnt);

			return;
		}
		case EventType::EvOnCollisionStay:
		{
			//Set entity to either first or second ID depending on which it isn't
			Entity otherEnt = ev.GetThis().m_entity;

			if (ev.GetThis().m_entity == m_entity)
				otherEnt = ev.GetOther().m_entity;

			if (ev.GetEventCat() & EventCatTrigger)
				OnTriggerStay(otherEnt);
			else
				OnCollisionStay(otherEnt);

			return;
		}
		case EventType::EvOnCollisionExit:
		{
			//Set entity to either first or second ID depending on which it isn't
			Entity otherEnt = ev.GetThis().m_entity;

			if (ev.GetThis().m_entity == m_entity)
				otherEnt = ev.GetOther().m_entity;

			if (ev.GetEventCat() & EventCatTrigger)
				OnTriggerExit(otherEnt);
			else
				OnCollisionExit(otherEnt);

			return;
		}
		}
	}

	void ScriptComponent::OnCollisionStay(Entity otherEnt)
	{
		//RE_INFO("COLLISION STAY");
	}

	void ScriptComponent::OnCollisionExit(Entity otherEnt)
	{
		//RE_INFO("COLLISION EXIT");
	}

	void ScriptComponent::OnTriggerEnter(Entity otherEnt)
	{
		//RE_INFO("TRIGGER ENTER");
		auto& hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(otherEnt);
		if (hierarchyObj.m_objectName == "Player")
		{
			PLAYER_STATUS.SetInLightDur();
		}
	}

	void ScriptComponent::OnTriggerStay(Entity otherEnt)
	{
		//RE_INFO("TRIGGER STAY");
	}

	void ScriptComponent::OnTriggerExit(Entity otherEnt)
	{
		//RE_INFO("TRIGGER EXIT");
	}

	std::shared_ptr<LogicComponent> ScriptComponent::GetLogicComponent()
	{
		return m_logicComponent;
	}

	void ScriptComponent::SetLogicComponent(LogicComponent& logicComp)
	{
		m_logicComponent = std::make_shared<LogicComponent>(logicComp);
	}

	std::shared_ptr<StatsComponent> ScriptComponent::GetStatsComponent()
	{
		return m_statsComponent;
	}

	void ScriptComponent::SetStatsComponent(StatsComponent& statsComp)
	{
		m_statsComponent = std::make_shared<StatsComponent>(statsComp);
	}

	void ScriptComponent::AddNextPoint(Vec2 newPoint)
	{
		m_nextPoint.push(newPoint);
	}

	Vec2 ScriptComponent::GetNextPoint()
	{
		return m_nextPoint.front();
	}

	void ScriptComponent::PopNextPoint()
	{
		m_nextPoint.pop();
	}

	bool ScriptComponent::NextPointEmpty()
	{
		return m_nextPoint.empty();
	}

}