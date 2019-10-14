#pragma once

class Timer
{
public:
	Timer() = default;
	void TimerInit(const char* System);
	void TimerEnd(const char* System);
private:
	std::map<const char*, float> time;
	std::chrono::time_point<std::chrono::steady_clock> StartTimer;
	float totaltime;
	std::chrono::high_resolution_clock timer;
};