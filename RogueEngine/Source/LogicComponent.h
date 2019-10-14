#pragma once
#include "AIType.h"
#include <vector>
#include <bitset>

class LogicComponent
	: public BaseComponent
{
public:
	LogicComponent() = default;
	~LogicComponent() = default;

	std::vector<AIState> AllStates();
	AIState CurState() const;
	void	CurState(AIState newState);
	AIType	LogicType() const;
	void	LogicType(AIType newType);
	void	SetActiveStateBit(size_t pos);
	bool	GetActiveStateBit(size_t pos) const;
	void	AddAIState(AIState newState);
	void	AddAIStateInactive(AIState newState);

	std::vector<AIState> AllAIStates() const;

	//From BaseComponent
	//ISerializable
	std::string Serialize();
	void Deserialize(std::string toDeserialize);

private:

	enum AIType m_AIType;
	//AllStates must be a vector since it does not necessarily act in order
	std::vector<AIState> m_allStates;
	//Multiple states can be active, but the first active state will be executed
	std::bitset<static_cast<size_t>(AIState::AIState_Last)> m_activeStates;
	enum AIState m_currentState;
};

