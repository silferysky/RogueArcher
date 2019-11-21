#include "Precompiled.h"

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

		for (auto it = m_allStates.rbegin(); it != m_allStates.rend();)
		{
			ss << static_cast<int>(*it);
			if (++it != m_allStates.rend())
				ss << ";";
		}

		return ss.str();
	}

	void LogicComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
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
				m_allStates.emplace(m_allStates.begin(),static_cast<AIState>(stoi(s1)));
				break;
			}
			++counter;
		}

		//std::swap(m_allStates,allStates);
	}

	void LogicComponent::DisplayOnInspector()
	{
		const char* aiType[] = { "Static", "Patrol", "Finder", "Trigger", "Transition" };
		const char* aiState[] = { "Idle", "Chase", "Patrol"};
		int tempInt = (int)m_AIType;

		//For AI Type
		ImGui::Combo("AI Type", &tempInt, aiType, IM_ARRAYSIZE(aiType));
		m_AIType = (AIType)tempInt;
		
		//For initial state
		tempInt = (int)m_currentState;
		ImGui::Combo("Initial State", &tempInt, aiType, IM_ARRAYSIZE(aiState));
		m_currentState = (AIState)tempInt;

		//For all states
		std::ostringstream ostrstream;
		size_t count = 1;
		for (AIState& state : m_allStates)
		{
			tempInt = (int)state;
			ostrstream.clear();
			ostrstream.str("");
			ostrstream << "State " << count;
			ImGui::PushItemWidth(75);
			ImGui::Combo(ostrstream.str().c_str(), &tempInt, aiState, IM_ARRAYSIZE(aiState));
			state = (AIState)tempInt;
			++count;
		}

		if (ImGui::Button("Add New State"))
		{
			m_allStates.push_back(AIState::AIState_Idle);
		}

		if (ImGui::Button("Remove State"))
		{
			if (m_allStates.size())
				m_allStates.pop_back();
		}
	}

}