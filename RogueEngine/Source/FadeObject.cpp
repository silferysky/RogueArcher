#include "Precompiled.h"
#include "FadeObject.h"
#include "GraphicsEvent.h"
#include "EventDispatcher.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	FadeObject::FadeObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, bool isContinuous, bool active, float maxTimer)
		: ScriptComponent(entity, logicComponent, statsComponent), m_continuous{ isContinuous }, m_active{ active }, m_maxTimer{ maxTimer }, m_timer{ 0.0f }
	{
	}

	void FadeObject::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void FadeObject::AIIdleUpdate()
	{
		if (!m_active)
			return;

		m_timer += g_deltaTime * g_engine.GetTimeScale();
		if (m_timer > m_maxTimer)
		{
			FadeEvent ev (m_entity, m_maxTimer);
			ev.SetSystemReceivers(static_cast<int>(SystemID::id_FADESYSTEM));
			EventDispatcher::instance().AddEvent(ev);

			m_timer = 0.0f;
			if (!m_continuous)
				m_active = false;
		}
	}

	void FadeObject::ResetTimer()
	{
		m_timer = 0.0f;
	}

	void FadeObject::SetMaxTimer(float max)
	{
		m_maxTimer = max;
	}

	void FadeObject::SetIsActive(bool active)
	{
		m_active = active;
	}

	void FadeObject::SetContinuous(bool isContinuous)
	{
		m_continuous = isContinuous;
	}

	float FadeObject::GetTimer() const
	{
		return m_timer;
	}
	float FadeObject::GetMaxTimer() const
	{
		return m_maxTimer;
	}
	bool FadeObject::IsActive() const
	{
		return m_active;
	}
	bool FadeObject::IsContinuous() const
	{
		return m_continuous;
	}
}