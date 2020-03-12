/* Start Header ************************************************************************/
/*!
\file           TriggerAI.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for TriggerAI

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "TriggerAI.h"

namespace Rogue
{
	TriggerAI::TriggerAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_isTriggered{ false } {}

	void TriggerAI::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		auto& hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(other);

		if (hierarchyObj.m_objectName != "Player" || m_isTriggered)
			return;
		
		m_isTriggered = true;

		if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(m_entity))
		{
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setTexturePath("Resources/Assets/CrystalFlowerClose.png");
			g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity).setTexture("Resources/Assets/CrystalFlowerClose.png");
		}

		if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(m_entity))
		{
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setCurrentFrame(0);
			g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity).setIsLooping(false);
		}

		g_engine.m_coordinator.GetSystem<LogicSystem>()->TriggerNextDoor();

		ScriptComponent::OnTriggerEnter(other);
		/*auto& hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(other);
		if (hierarchyObj.m_objectName == "Ball")
		{
			//ClearTimedEntities already deletes entity
			g_engine.m_coordinator.ClearTimedEntities();
		}*/
	}
}