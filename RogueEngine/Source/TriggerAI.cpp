#include "Precompiled.h"
#include "TriggerAI.h"
#include "Main.h"

namespace Rogue
{
	TriggerAI::TriggerAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: BaseAI(entity, logicComponent, statsComponent), m_isTriggered{ false } {}

	void TriggerAI::OnTriggerEnter(Entity other)
	{
		auto& hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(other);

		if (hierarchyObj.m_objectName != "Ball" || m_isTriggered)
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

		BaseAI::OnTriggerEnter(other);
		/*auto& hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(other);
		if (hierarchyObj.m_objectName == "Ball")
		{
			//ClearTimedEntities already deletes entity
			g_engine.m_coordinator.ClearTimedEntities();
		}*/
	}
}