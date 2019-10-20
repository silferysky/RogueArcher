#pragma once
#include <chrono>
#include <map>

class Timer
{
public:
	Timer() = default;
	void TimerInit(const char* System);
	void TimerEnd(const char* System);
	std::map<const char*, float> GetTimeSystem() const;
private:
	std::map<const char*, float> time;
	std::chrono::time_point<std::chrono::steady_clock> StartTimer;
	float totaltime;
	std::chrono::high_resolution_clock timer;
};