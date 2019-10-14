#include "BaseAI.h"

BaseAI::BaseAI(LogicComponent& logicComp)
{
	m_logicComponent = std::make_shared<LogicComponent>(logicComp);
}

BaseAI::~BaseAI()
{
}

void BaseAI::logicUpdate()
{
}

std::shared_ptr<LogicComponent> BaseAI::getLogicComponent()
{
	return m_logicComponent;
}

void BaseAI::setLogicComponent(LogicComponent& logicComp)
{
	m_logicComponent = std::make_shared<LogicComponent>(logicComp);
}
