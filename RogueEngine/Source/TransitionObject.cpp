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
//#include "Main.h"
#include "GraphicsEvent.h"
#include "CameraManager.h"

namespace Rogue
{
	TransitionObject::TransitionObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, TransformComponent& transComponent, const std::string& levelToLoad)
		: ScriptComponent(entity, logicComponent, statsComponent), m_levelToLoad{ levelToLoad }, m_trans{ transComponent }, m_black{ MAX_ENTITIES } {}

	void TransitionObject::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (m_black < MAX_ENTITIES)
		{
			if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_black))
			{
				if (fade->get().getIsActive() == false)
					g_engine.m_coordinator.SetTransition(true);
			}
		}
	}

	void TransitionObject::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			PLAYER_STATUS.SetIndicatorStatus(false);
			g_engine.m_coordinator.SetTransitionLevel(m_levelToLoad.c_str(), 0.0f); //2nd value doesn't matter anymore probably
			
			m_black = PLAYER_STATUS.CreateFadeObject(true);
			PLAYER_STATUS.SetFadeTransition(true);
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