#include <time.h>
#include <ctime>
#include <chrono>
#include "Main.h"
#include "glew.h"
#include <unordered_map>

double deltaTime;
double oldTime;
float dt();
void setVSync(bool sync);
float fps(float dt);
class Timer
{
public:

	using clock_t = std::chrono::high_resolution_clock;
	//using duration_t = clock_t::duration;
	std::chrono::duration<double> TotalTime;
	Timer() :StartTime(clock_t::now()) { }

	void EndTime()
	{
		TotalTime = (clock_t::now() - StartTime); 
		TimesList[arraynumber] += TotalTime;
		IncreaseArray();
	}

	void IncreaseArray() 
	{
		++arraynumber;
	}

	void reset() { StartTime = clock_t::now(); }
	
private:
	std::vector<float> SystemTimes;
	clock_t::time_point StartTime;
	size_t arraynumber = 0;
	std::unordered_map<size_t, std::chrono::duration<double>> TimesList;

};