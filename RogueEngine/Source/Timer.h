#pragma once
#include <chrono>
#include <map>

namespace Rogue
{
	class Timer
	{
	public:
		using ChronoClock = std::chrono::high_resolution_clock;
		using ChronoTime = std::chrono::time_point <std::chrono::steady_clock>;
		using FloatSec = std::chrono::duration<float>;

		Timer() = default;
		void TimerInit(const char* System);
		void TimerEnd(const char* System);
		const std::map<const char*, float>& GetSystemTimes() const;
		static const float s_microToSeconds;
	private:
		ChronoClock m_timer;
		ChronoTime m_startTimer;

		std::map<const char*, float> m_timeMap;

	};
}