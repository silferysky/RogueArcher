#include "Timer.h"

void Timer::TimerInit(const char* System)
{
	time.insert(std::make_pair( System, totaltime ));
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
	for (auto it = time.begin(); it != time.end(); ++it)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}


}
