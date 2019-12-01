/* Start Header ************************************************************************/
/*!
\file           BaseAi.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BaseAi

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "BaseAI.h"
#include "Logger.h"
#include "Main.h"


namespace Rogue
{
	BaseAI::BaseAI(Entity entity, LogicComponent& logicComp, StatsComponent& statsComp)
		: m_entity{entity}, m_logicComponent{std::make_shared<LogicComponent>(logicComp)}, m_statsComponent{std::make_shared<StatsComponent>(statsComp)}
	{
		LogicInit();
	}

	void BaseAI::LogicInit()
	{
		//Put in order of importance
		//Use AddAIState for active states, AddAIStateInactive for inactive states that might turn active
		//m_logicComponent->AddAIState(AIState::AIState_Patrol);
		//m_logicComponent->AddAIState(AIState::AIState_Idle);

		//Sets initial state of AI
		//m_logicComponent->SetCurState(AIState::AIState_Idle);
	}

	void BaseAI::LogicUpdate()
	{
		//To set all flags of potential behavior
		m_logicComponent->ResetActiveStateBit();
		AIActiveStateUpdate();

		//For all possible states BaseAI has
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

	void BaseAI::AIDetect()
	{
	}

	void BaseAI::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit((size_t)AIState::AIState_Idle);
	}

	void BaseAI::AIChaseUpdate()
	{
		//RE_INFO("CHASING PLAYER");
	}

	void BaseAI::AIPatrolUpdate()
	{
		//RE_INFO("AI PATROLLING");
	}

	void BaseAI::AIIdleUpdate()
	{
		//RE_INFO("AI IDLE");
	}

	void BaseAI::HandleCollision(EntCollisionOrTrigger* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvOnCollisionEnter:
		{
			//Set entity to either first or second ID depending on which it isn't
			Entity otherEnt = ev->GetEntityID();

			if (ev->GetEntityID() == m_entity)
				otherEnt = ev->GetOtherEntity();

			if (ev->GetEventCat() & EventCatTrigger)
				OnTriggerEnter(otherEnt);
			else
				OnCollisionEnter(otherEnt);

			return;
		}
		case EventType::EvOnCollisionStay:
		{
			//Set entity to either first or second ID depending on which it isn't
			Entity otherEnt = ev->GetEntityID();

			if (ev->GetEntityID() == m_entity)
				otherEnt = ev->GetOtherEntity();

			if (ev->GetEventCat() & EventCatTrigger)
				OnTriggerStay(otherEnt);
			else
				OnCollisionStay(otherEnt);

			return;
		}
		case EventType::EvOnCollisionExit:
		{
			//Set entity to either first or second ID depending on which it isn't
			Entity otherEnt = ev->GetEntityID();

			if (ev->GetEntityID() == m_entity)
				otherEnt = ev->GetOtherEntity();

			if (ev->GetEventCat() & EventCatTrigger)
				OnTriggerExit(otherEnt);
			else
				OnCollisionExit(otherEnt);

			return;
		}
		}
	}

	void BaseAI::OnCollisionEnter(Entity otherEnt)
	{
		//RE_INFO("COLLISION ENTER");
	}

	void BaseAI::OnCollisionStay(Entity otherEnt)
	{
		//RE_INFO("COLLISION STAY");
	}

	void BaseAI::OnCollisionExit(Entity otherEnt)
	{
		//RE_INFO("COLLISION EXIT");
	}

	void BaseAI::OnTriggerEnter(Entity otherEnt)
	{
		//RE_INFO("TRIGGER ENTER");
		auto& hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(otherEnt);
		if (hierarchyObj.m_objectName == "Ball")
		{
			//ClearTimedEntities already deletes entity
			g_engine.m_coordinator.ClearTimedEntities();
		}
		else if (hierarchyObj.m_objectName == "Player")
		{
			g_engine.m_coordinator.GetSystem<PlayerControllerSystem>()->setInLight();
		}
	}

	void BaseAI::OnTriggerStay(Entity otherEnt)
	{
		//RE_INFO("TRIGGER STAY");
	}

	void BaseAI::OnTriggerExit(Entity otherEnt)
	{
		//RE_INFO("TRIGGER EXIT");
	}

	std::shared_ptr<LogicComponent> BaseAI::GetLogicComponent()
	{
		return m_logicComponent;
	}

	void BaseAI::SetLogicComponent(LogicComponent& logicComp)
	{
		m_logicComponent = std::make_shared<LogicComponent>(logicComp);
	}

	std::shared_ptr<StatsComponent> BaseAI::GetStatsComponent()
	{
		return m_statsComponent;
	}

	void BaseAI::SetStatsComponent(StatsComponent& statsComp)
	{
		m_statsComponent = std::make_shared<StatsComponent>(statsComp);
	}

	void BaseAI::AddNextPoint(Vec2 newPoint)
	{
		m_nextPoint.push(newPoint);
	}

	Vec2 BaseAI::GetNextPoint()
	{
		return m_nextPoint.front();
	}

	void BaseAI::PopNextPoint()
	{
		m_nextPoint.pop();
	}

	bool BaseAI::NextPointEmpty()
	{
		return m_nextPoint.empty();
	}

}