#include <vector>
#include <sstream>

#include "LogicComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	std::vector<AIState> LogicComponent::GetAllAIStates() const
	{
		return m_allStates;
	}

	void LogicComponent::AddAIState(AIState newState)
	{
		//Assumes the new state is not in yet
		m_allStates.push_back(newState);
		m_activeStates.set(static_cast<size_t>(newState), true);
	}

	void LogicComponent::AddAIStateInactive(AIState newState)
	{
		//Assumes the new state is not in yet
		m_allStates.push_back(newState);
	}

	void LogicComponent::SetAllAIStates(std::vector<AIState> states)
	{
		std::swap(states, m_allStates);
	}

	AIState LogicComponent::GetCurState() const
	{
		return m_currentState;
	}

	void LogicComponent::SetCurState(AIState newState)
	{
		m_currentState = newState;
	}

	AIType LogicComponent::GetLogicType() const
	{
		return m_AIType;
	}

	void LogicComponent::SetLogicType(AIType newType)
	{
		m_AIType = newType;
	}

	void LogicComponent::SetActiveStateBit(size_t pos)
	{
		m_activeStates.set(pos);
	}

	void LogicComponent::ResetActiveStateBit()
	{
		m_activeStates.reset();
	}

	bool LogicComponent::GetActiveStateBit(size_t pos) const
	{
		return m_activeStates.test(pos);
	}

	std::string LogicComponent::Serialize()
	{
		//Acceleration, Velocity, Mass, Volume, isStatic
		std::ostringstream ss;
		ss << static_cast<int>(m_AIType) << ";";
		ss << static_cast<int>(m_currentState) << ";";

		for (auto it = m_allStates.begin(); it != m_allStates.end();)
		{
			ss << static_cast<int>(*it);
			if (++it != m_allStates.end())
				ss << ";";
		}

		return ss.str();
	}

	void LogicComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;
		int counter = 0;		//Needed to take in for multiple values
		//std::vector<AIState> allStates{};

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
				m_allStates.push_back(static_cast<AIState>(stoi(s1)));
				break;
			}
			++counter;
		}

		//std::swap(m_allStates,allStates);
	}

}