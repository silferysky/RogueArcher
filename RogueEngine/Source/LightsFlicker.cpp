#include "Precompiled.h"
#include "LightsFlicker.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	LightsFlicker::LightsFlicker(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void LightsFlicker::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void LightsFlicker::AIIdleUpdate()
	{
		auto& light = g_engine.m_coordinator.GetComponent<LightComponent>(m_entity);

		light.setAmbientFactor(rand() / (RAND_MAX / 52));
		light.setSpecularFactor(rand() / (RAND_MAX / 52));
	}
}