#include "Precompiled.h"
#include "LightsFlicker.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	LightsFlicker::LightsFlicker(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void LightsFlicker::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void LightsFlicker::AIIdleUpdate()
	{
		m_timer += g_deltaTime * g_engine.GetTimeScale();

		if (m_timer > static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.25f))
		{
			if (auto light = g_engine.m_coordinator.TryGetComponent<LightComponent>(m_entity))
			{
				light->get().setAmbientFactor(static_cast<float>(rand() / (RAND_MAX / 17)));
				light->get().setSpecularFactor(static_cast<float>(rand() / (RAND_MAX / 17)));

				m_timer = 0.0f;
			}
		}
	}
}