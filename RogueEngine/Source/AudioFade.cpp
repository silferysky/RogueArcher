/* Start Header ************************************************************************/
/*!
\file           FadeObject.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for FadeObject

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "AudioFade.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	AudioFade::AudioFade(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, bool isContinuous, bool active, float maxTimer)
		: ScriptComponent(entity, logicComponent, statsComponent), m_timer{ EPSILON }, m_originalVolume{ 0.3f }
	{
		if (auto sound = g_engine.m_coordinator.TryGetComponent<AudioEmitterComponent>(m_entity))
		{
			m_originalVolume = sound->get().getVolume();
		}
	}

	void AudioFade::AIActiveStateUpdate()
	{
		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void AudioFade::AIIdleUpdate()
	{
		m_timer += g_deltaTime;

		if (auto sound = g_engine.m_coordinator.TryGetComponent<AudioEmitterComponent>(m_entity))
		{
			if (m_timer < MAX_TIMER)
			{
				sound->get().setVolume(m_originalVolume * m_timer / MAX_TIMER);
				std::cout << "AUDIO VOLUME" << m_originalVolume * m_timer / MAX_TIMER << std::endl;
			}
			else
				sound->get().setVolume(m_originalVolume);
		}
	}

	void AudioFade::ResetTimer()
	{
		m_timer = EPSILON;
	}
}