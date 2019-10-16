#include "BaseAI.h"
#include "Logger.h"
#include "Main.h"

BaseAI::BaseAI(LogicComponent& logicComp)
{
	m_logicComponent = std::make_shared<LogicComponent>(logicComp);
	logicInit();
}

BaseAI::~BaseAI()
{
}

void BaseAI::logicInit()
{
	//Put in order of importance
	//Use AddAIState for active states, AddAIStateInactive for inactive states that might turn active
	m_logicComponent->AddAIState(AIState::AIState_Idle);

	//Sets initial state of AI
	m_logicComponent->CurState(AIState::AIState_Idle);
}

void BaseAI::logicUpdate()
{
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
			//Do Stuff
			RE_INFO("Base AI Doing Stuff");
			RE_INFO(static_cast<int>(*it));

			//Sets current state and exit state since you aren't supposed to do multiple states
			m_logicComponent->CurState(*it);
			break;
		}
	}
}

std::shared_ptr<LogicComponent> BaseAI::getLogicComponent()
{
	return m_logicComponent;
}

void BaseAI::setLogicComponent(LogicComponent& logicComp)
{
	m_logicComponent = std::make_shared<LogicComponent>(logicComp);
}
