#include "BaseAI.h"
#include "Logger.h"
#include "Main.h"


namespace Rogue
{
	BaseAI::BaseAI(LogicComponent& logicComp)
	{
		m_logicComponent = std::make_shared<LogicComponent>(logicComp);
		logicInit();
	}

	void BaseAI::logicInit()
	{
		//Put in order of importance
		//Use AddAIState for active states, AddAIStateInactive for inactive states that might turn active
		m_logicComponent->AddAIStateInactive(AIState::AIState_Chase);
		m_logicComponent->AddAIState(AIState::AIState_Patrol);
		m_logicComponent->AddAIState(AIState::AIState_Idle);

		//Sets initial state of AI
		m_logicComponent->CurState(AIState::AIState_Idle);
	}

	void BaseAI::logicUpdate()
	{
		//To set all flags of potential behavior
		m_logicComponent->ResetActiveStateBit();
		AIActiveStateUpdate();
		AIDetect();

		//For all possible states BaseAI has
		//This for loop handles the order of importance of each state.
		for (auto it = m_logicComponent->AllAIStates().begin(); it != m_logicComponent->AllAIStates().end(); ++it)
		{
			//If its ActiveStateBit matches, run that update
			if (m_logicComponent->GetActiveStateBit(static_cast<size_t>(*it)))
			{
				switch (*it)
				{
				case AIState::AIState_Chase:
					AIChaseUpdate();
					break;
				case AIState::AIState_Look:
					AILookUpdate();
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
				m_logicComponent->CurState(*it);
				//Break when done so other states would not be performed
				break;
			}
		}
	}

	void BaseAI::AIDetect()
	{
		/*auto gameFactory = gEngine.m_coordinator.GetSystem<ObjectFactory>();
		for (auto it = gameFactory->GetActiveEntity().begin(); it != gameFactory->GetActiveEntity().end(); ++it)
		{
			if (gEngine.m_coordinator.CheckIfComponentExists<PlayerControllerComponent>(*it))
			{
				//if(gEngine.m_coordinator.get)
				RE_INFO("SPOTTED THROUGH WALLS");
				break;
			}
		}*/
	}

	void BaseAI::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit((size_t)AIState::AIState_Idle);
	}

	std::shared_ptr<LogicComponent> BaseAI::getLogicComponent()
	{
		return m_logicComponent;
	}

	void BaseAI::setLogicComponent(LogicComponent& logicComp)
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