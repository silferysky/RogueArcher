/* Start Header ************************************************************************/
/*!
\file           Timer.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Timer

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "Main.h"
#include "Timer.h"

namespace Rogue
{
	// Units
	const float Timer::s_microsecondsPerSecond = 1000000.0f; // To be used at the final calculation
	const float Timer::s_millisecondsPerSecond = 1000.0f; // To be used at the final calculation
	const float Timer::s_microsecondsPerMillisecond = 1000.0f; // To be used at the final calculation
		
	void Timer::TimerInit(const char* System)
	{
		m_timeMap.insert(std::make_pair(System, 0.0f));
		m_startTimer = m_timer.now();
	}

	void Timer::TimerEnd(const char* SystemName)
	{
		auto EndTimer = m_timer.now();
		FloatSec delta = EndTimer - m_startTimer;

		auto i = std::find_if(m_timeMap.begin(), m_timeMap.end(),
			[&SystemName](const std::pair<const char*, float>& element) { return element.first == SystemName; });

		RE_ASSERT(i != m_timeMap.end(), "Cannot find system in timer!");
		
		i->second = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(delta).count());
	}

	const std::map<const char*, float>& Timer::GetSystemTimes() const
	{
		return m_timeMap;
	}

	Timer::ChronoTime Timer::GetCurrTime() const
	{
		return m_timer.now();
	}
}
