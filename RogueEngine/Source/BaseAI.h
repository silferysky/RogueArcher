#pragma once
#include "ILogic.h"
#include "LogicComponent.h"
#include <memory>

class BaseAI
	: public ILogic
{
public:

	BaseAI(LogicComponent& logicComp);
	~BaseAI();

	//From ILogic
	void logicUpdate();

	//Getter/Setter
	std::shared_ptr<LogicComponent> getLogicComponent();
	void setLogicComponent(LogicComponent& logicComp);

private:
	std::shared_ptr<LogicComponent> m_logicComponent;
};