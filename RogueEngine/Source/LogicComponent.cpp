#include "LogicComponent.h"
#include "Vector2D.h"
#include <sstream>

std::vector<AIState> LogicComponent::AllStates()
{
	return m_allStates;
}

AIState LogicComponent::CurState() const
{
	return m_currentState;
}

void LogicComponent::CurState(AIState newState)
{
	m_currentState = newState;
}

AIType LogicComponent::LogicType() const
{
	return m_AIType;
}

void LogicComponent::LogicType(AIType newType)
{
	m_AIType = newType;
}

std::string LogicComponent::Serialize()
{
	//Acceleration, Velocity, Mass, Volume, isStatic
	std::ostringstream ss;
	ss << static_cast<int>(m_AIType) << ";";
	ss << static_cast<int>(m_currentState) << ";";

	for (auto it = m_allStates.begin();;)
	{
		ss << static_cast<int>(*it);
		if (++it != m_allStates.end())
			ss << ";";
		else
			break;
	}

	return ss.str();
}

void LogicComponent::Deserialize(std::string toDeserialize)
{
	std::istringstream ss(toDeserialize);
	std::string s1;	
	int counter = 0;		//Needed to take in for multiple values
	std::vector<AIState> allStates{};
	size_t size = 0;

	while (std::getline(ss, s1, ';'))
	{
		switch (counter)
		{
		case 0:
		{
			m_AIType = static_cast<AIType>(stoi(s1));
			break;
		}
		case 1:
		{
			m_currentState = static_cast<AIState>(stoi(s1));
			break;
		}
		default:
			allStates.push_back(static_cast<AIState>(stoi(s1)));
			break;
		}
		++counter;
	}

	m_allStates = allStates;
}
