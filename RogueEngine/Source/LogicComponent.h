#pragma once
#include "AIType.h"

class LogicComponent
	: public BaseComponent
{
	LogicComponent() = default;
	~LogicComponent() = default;

	std::vector<AIState> AllStates();
	AIState CurState() const;
	void	CurState(AIState newState);
	AIType	LogicType() const;
	void	LogicType(AIType newType);

	//From BaseComponent
	//ISerializable
	std::string Serialize();
	void Deserialize(std::string toDeserialize);

private:

	enum AIType m_AIType;
	std::vector<AIState> m_allStates;
	enum AIState m_currentState;
};

