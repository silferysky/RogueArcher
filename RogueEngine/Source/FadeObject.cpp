#include "FadeObject.h"
#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	FadeObject::FadeObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, bool isContinuous, bool startsByFadingIn, float maxTimer)
		: ScriptComponent(entity, logicComponent, statsComponent), m_continuous{ isContinuous }, m_fadingin{ startsByFadingIn }, m_maxTimer{ maxTimer }, m_timer{ 0.0f }
	{
	}

	void FadeObject::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void FadeObject::AIIdleUpdate()
	{
		m_timer += g_deltaTime * g_engine.GetTimeScale();
		if (m_timer > m_maxTimer)
		{

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

	void FadeObject::SetIsFadingIn(bool isFadingIn)
	{
		m_fadingin = isFadingIn;
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
	bool FadeObject::IsFadingIn() const
	{
		return m_fadingin;
	}
	bool FadeObject::IsContinuous() const
	{
		return m_continuous;
	}
}