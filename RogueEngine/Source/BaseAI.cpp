#include "BaseAI.h"
#include "Logger.h"
#include "Main.h"


namespace Rogue
{
	BaseAI::BaseAI(Entity entity, LogicComponent& logicComp)
	{
		m_entity = entity;
		m_logicComponent = std::make_shared<LogicComponent>(logicComp);
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
		AIDetect();

		//For all possible states BaseAI has
		//This for loop handles the order of importance of each state.
		auto allStatesVector = m_logicComponent->GetAllAIStates();
		for (auto it = allStatesVector.begin(); it != allStatesVector.end(); ++it)
		{
			//If its ActiveStateBit matches, run that update
			if (m_logicComponent->GetActiveStateBit(static_cast<size_t>(*it)))
			{
				switch (*it)
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
				m_logicComponent->SetCurState(*it);
				//Break when done so other states would not be performed
				break;
			}
		}
	}

	void BaseAI::AIDetect()
	{
		
		/*auto gameFactory = g_engine.m_coordinator.GetSystem<ObjectFactory>();
		for (auto it = gameFactory->GetActiveEntity().begin(); it != gameFactory->GetActiveEntity().end(); ++it)
		{
			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(*it))
			{
				//if(g_engine.m_coordinator.get)
				RE_INFO("SPOTTED THROUGH WALLS");
				break;
			}
		}*/
	}

	void BaseAI::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit((size_t)AIState::AIState_Idle);
		m_logicComponent->SetActiveStateBit((size_t)AIState::AIState_Chase);
	}

	void BaseAI::AIChaseUpdate()
	{
		RE_INFO("CHASING PLAYER");
	}

	void BaseAI::AIPatrolUpdate()
	{
		RE_INFO("AI PATROLLING");
	}

	void BaseAI::AIIdleUpdate()
	{
		RE_INFO("AI IDLE");
	}

	std::shared_ptr<LogicComponent> BaseAI::GetLogicComponent()
	{
		return m_logicComponent;
	}

	void BaseAI::SetLogicComponent(LogicComponent& logicComp)
	{
		m_logicComponent = std::make_shared<LogicComponent>(logicComp);
	}

	void BaseAI::AddWaypoint(Vec2 newPoint)
	{
		m_waypoints.push_back(newPoint);
	}

	void BaseAI::ClearWaypoints()
	{
		m_waypoints.clear();
	}

	std::vector<Vec2> BaseAI::GetWaypoints()
	{
		return m_waypoints;
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