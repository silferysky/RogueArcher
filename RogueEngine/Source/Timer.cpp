#include "Main.h"
#include "Timer.h"

namespace Rogue
{
	const float Timer::s_microToSeconds = 1000000.0f; // To be used at the final calculation

	void Timer::TimerInit(const char* System)
	{
		m_timeMap.insert(std::make_pair(System, m_totalTime));
		m_startTimer = m_timer.now();
	}

	void Timer::TimerEnd(const char* SystemName)
	{
		auto EndTimer = m_timer.now();

		m_totalTime = std::chrono::duration_cast<std::chrono::microseconds>(EndTimer - m_startTimer).count();

		auto i = std::find_if(m_timeMap.begin(), m_timeMap.end(),
			[&SystemName](const std::pair<const char*, float>& element) { return element.first == SystemName; });

		RE_ASSERT(i != m_timeMap.end(), "Cannot find system in timer!");
		
		i->second = m_totalTime;
	}

	const std::map<const char*, float>& Timer::GetSystemTimes() const
	{
		return m_timeMap;
	}
}
