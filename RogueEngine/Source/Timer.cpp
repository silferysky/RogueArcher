#include <chrono>
#include <map>
#include <iterator> 

#include "Main.h"
#include "Timer.h"

namespace Rogue
{
	void Timer::TimerInit(const char* System)
	{
		time.insert(std::make_pair(System, totaltime));
		StartTimer = timer.now();
	}

	void Timer::TimerEnd(const char* System)
	{
		auto EndTimer = timer.now();
		totaltime = std::chrono::duration_cast<std::chrono::microseconds>(EndTimer - StartTimer).count() / 1000000.0f;
		totaltime = (totaltime / gDeltaTime) * 100;
		std::map<const char*, float>::iterator it = time.find(System);
		if (it != time.end())
		{
			it->second = totaltime;
		}
		for (auto i : time)
		{
			std::cout << i.first << i.second << std::endl;
		}
	}

	const std::map<const char*, float> Timer::GetTimeSystem() const
	{
		const std::map<const char*, float>& temp = time;
		return temp;
	}
}

