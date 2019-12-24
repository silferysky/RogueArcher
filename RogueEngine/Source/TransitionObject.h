/* Start Header ************************************************************************/
/*!
\file           TransitionObject.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for TransitionObject

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
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
	class TransitionObject
		: public ScriptComponent
	{
	public:
		TransitionObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, const std::string& levelToLoad = "Level 1.json");

		virtual void OnTriggerEnter(Entity other) override;

		std::string& GetTransitionLevelName();
		void SetTransitionLevelName(std::string name);

	private:
		std::string m_levelToLoad;
	};
}