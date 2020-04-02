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

namespace Rogue
{
	TransitionObject::TransitionObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, TransformComponent& transComponent, const std::string& levelToLoad)
		: ScriptComponent(entity, logicComponent, statsComponent), m_levelToLoad{ levelToLoad }, m_trans{ transComponent } {}

	void TransitionObject::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			PLAYER_STATUS.SetIndicatorStatus(false);
			g_engine.m_coordinator.SetTransitionLevel(m_levelToLoad.c_str());

			FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f);
			ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
			EventDispatcher::instance().AddEvent(ev);

			Entity blackEntity = g_engine.m_coordinator.CreateEntity();
			auto& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(blackEntity);
			auto& trans = g_engine.m_coordinator.CreateComponent<TransformComponent>(blackEntity);
			auto& fade = g_engine.m_coordinator.CreateComponent<FadeComponent>(blackEntity);

			sprite.setTexture("Resources/Assets/Black.png");
			auto& rgba = sprite.getFilter();
			rgba.a = 0.0f;
			sprite.setFilter(rgba);
			trans.setPosition(m_trans.GetPosition());
			trans.setScale(Vec2(3000.0f, 2000.0f));
			trans.setZ(10000.0f);
			fade.setIsActive(true);
			fade.setIsFadingIn(true);
			fade.setFadeVelocity(1.0f);
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