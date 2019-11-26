#include "Precompiled.h"
#include "TriggerAI.h"
#include "Main.h"

namespace Rogue
{
	TriggerAI::TriggerAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: BaseAI(entity, logicComponent, statsComponent) {}

	void TriggerAI::OnTriggerEnter(Entity other)
	{
		auto& hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(other);
		if (hierarchyObj.m_objectName == "Ball")
		{
			g_engine.m_coordinator.DestroyEntity(other);
		}
	}
}