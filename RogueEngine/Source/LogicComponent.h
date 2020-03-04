/* Start Header ************************************************************************/
/*!
\file           LogicComponent.h
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
#pragma once
#include "AIType.h"
#include "BaseComponent.h"
#include <vector>
#include <bitset>

namespace Rogue
{
	class LogicComponent
		: public BaseComponent
	{
	public:
		LogicComponent() = default;
		~LogicComponent() = default;

		std::vector<AIState> GetAllAIStates() const;
		void	SetAllAIStates(std::vector<AIState> states);
		void	AddAIState(AIState newState);
		void	AddAIStateInactive(AIState newState);

		//std::vector<std::string> GetScriptString() const;
		//void	AddScriptString(const std::string& newScript);

		AIState GetCurState() const;
		void	SetCurState(AIState newState);

		AIType	GetLogicType() const;
		void	SetLogicType(AIType newType);

		void	SetActiveStateBit(size_t pos);
		void	ResetActiveStateBit();
		bool	GetActiveStateBit(size_t pos) const;

		//From BaseComponent
		//ISerializable
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();

	private:

		enum AIType m_AIType;
		//AllStates must be a vector since it does not necessarily act in order
		std::vector<AIState> m_allStates;
		//std::vector<std::string> m_allScripts;
		//Multiple states can be active, but the first active state will be executed
		std::bitset<static_cast<size_t>(AIState::AIState_Last)> m_activeStates;
		enum AIState m_currentState;
		bool m_isActive;
	};
}