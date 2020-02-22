/* Start Header ************************************************************************/
/*!
\file           TransitionObject.cpp
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
#include "Precompiled.h"
#include "TransitionObject.h"
#include "Main.h"

namespace Rogue
{
	TransitionObject::TransitionObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, const std::string& levelToLoad)
		: ScriptComponent(entity, logicComponent, statsComponent), m_levelToLoad{ levelToLoad } {}

	void TransitionObject::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			PLAYER_STATUS.SetIndicatorStatus(false);
			g_engine.m_coordinator.SetTransitionLevel(m_levelToLoad.c_str());

			//SceneManager& sceneManager = SceneManager::instance();
			//sceneManager.LoadLevel(m_levelToLoad.c_str());
		}
	}
	std::string& TransitionObject::GetTransitionLevelName()
	{
		return m_levelToLoad;
	}
	void TransitionObject::SetTransitionLevelName(std::string name)
	{
		m_levelToLoad = name;
	}
}