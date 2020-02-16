/* Start Header ************************************************************************/
/*!
\file           Timer.h
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
#pragma once
#include <chrono>
#include <map>

namespace Rogue
{
	class Timer
	{
	public:
		static Timer& instance()
		{
			static Timer instance;
			return instance;
		}

		using ChronoClock = std::chrono::high_resolution_clock;
		using ChronoTime = std::chrono::time_point <std::chrono::steady_clock>;
		using FloatSec = std::chrono::duration<float>;

		static const float s_microsecPerSec;
		static const float s_millisecPerSec;
		static const float s_microsecPerMillisec;

		Timer() = default;
		void TimerInit(const char* System);
		void TimerEnd(const char* System);
		std::map<const char*, float>& GetSystemTimes();

		ChronoTime GetCurrTime() const;
		float CalculateDuration(const ChronoTime& start, const ChronoTime& end) const;
	private:
		ChronoClock m_timer;
		ChronoTime m_startTimer;

		std::map<const char*, float> m_timeMap;

	};
}