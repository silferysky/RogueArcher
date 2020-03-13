/* Start Header ************************************************************************/
/*!
\file           LogicComponent.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for LogicComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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

	//std::vector<std::string> LogicComponent::GetScriptString() const
	//{
	//	return m_allScripts;
	//}

	// void LogicComponent::AddScriptString(const std::string& newScript)
	//{
	//	 m_allScripts.push_back(newScript);
	//}

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
		//AI Type, AI first state, all different AI states
		std::ostringstream ss;
		
		ss << static_cast<int>(m_AIType) << ";";
		ss << static_cast<int>(m_currentState) << ";";
		ss << static_cast<int>(m_allStates.size()) << ";";

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
		int numOfStates = 0;
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
			case 2:
			{
				numOfStates = stoi(s1);
				break;
			}
			default:
			{
				if (numOfStates > 0)
				{
					m_allStates.emplace(m_allStates.begin(), static_cast<AIState>(stoi(s1)));
					//m_allStates.push_back(static_cast<AIState>(stoi(s1)));
					--numOfStates;
				}
				//else
				//{
				//	m_allScripts.push_back(s1);
				//}
				break;
			}
			}
			++counter;
		}

		//std::swap(m_allStates,allStates);
	}

	void LogicComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);

		const char* aiType[] = { "Static", "Patrol", "Finder", "Trigger",  "Platform", "Transition", "Appear on Collide", "Disappear on Collide", 
			"UI Tele Charge 1", "UI Tele Charge 2", "UI Tele Charge 3", "Death", "Checkpoint", "Soul Collectible", "Teleport Animation", 
			"Animate on Exa", "Animate on Ela", "Activate on Exa", "Activate on Ela", "Activate on Exa Death", "Activate on Ela Death", "Lights Flicker", 
			"UI Hover Over Button", "UI Progress Bar", "Gameplay Ending", "Object Fade", "Zoom Trigger", "Emoji Script", "Trigger Emoji And Zoom",
		    "Vegetation Pillbug", "Vegetation Breakable", "Coral Clam", "Coral Bubble Transporter", "Coral Element Bubble", "Crystal Element Beetle",
			"Crystal Mole"};
		const char* aiState[] = { "Idle", "Chase", "Patrol"};
		int tempInt = static_cast<int>(m_AIType);

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