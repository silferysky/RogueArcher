/* Start Header ************************************************************************/
/*!
\file           TransitionScreen.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for TransitionScreen

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "ScriptComponent.h"
#include <string>

namespace Rogue
{
	class TransitionScreen
		: public ScriptComponent
	{
	public:
		TransitionScreen(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, const std::string& levelToLoad = "Level 1.json");

		virtual void AIActiveStateUpdate() override;

		std::string& GetTransitionLevelName();
		void SetTransitionLevelName(std::string name);

	private:
		std::string m_levelToLoad;
		float m_timer = 0.0f;
	};
}