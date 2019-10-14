#include "BaseAI.h"
#include "Logger.h"

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
	//Add to LogicSystem
}

void BaseAI::logicUpdate()
{
	int counter = 0;

	for (auto it = m_logicComponent->AllAIStates().begin(); it != m_logicComponent->AllAIStates().end(); ++it)
	{
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
			case default:
				//Idle is default state
				AIIdleUpdate();
				break;
			}
			//Do Stuff
			RE_INFO("Base AI Doing Stuff");
			RE_INFO((static_cast<int>(*it));

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
