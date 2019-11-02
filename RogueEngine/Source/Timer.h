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

		static const float s_microsecondsPerSecond;
		static const float s_millisecondsPerSecond;
		static const float s_microsecondsPerMillisecond;

		Timer() = default;
		void TimerInit(const char* System);
		void TimerEnd(const char* System);
		const std::map<const char*, float>& GetSystemTimes() const;

		ChronoTime GetCurrTime() const;
	private:
		ChronoClock m_timer;
		ChronoTime m_startTimer;

		std::map<const char*, float> m_timeMap;

	};
}