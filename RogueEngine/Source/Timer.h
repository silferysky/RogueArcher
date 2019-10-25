#pragma once
#include <chrono>
#include <map>

namespace Rogue
{
	class Timer
	{
	public:
		Timer() = default;
		void TimerInit(const char* System);
		void TimerEnd(const char* System);
		const std::map<const char*, float>& GetSystemTimes() const;
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_startTimer;
		std::chrono::high_resolution_clock m_timer;

		std::map<const char*, float> m_timeMap;
		float m_totalTime;

		static const float s_microToSeconds;
	};
}