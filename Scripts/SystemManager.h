#pragma once
#include <list>
#include "System.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include "EventManager.h"
//#include "AudioManager.h"

class SystemManager
{
public:
	SystemManager();
	~SystemManager();

	void AddSystem(System* Sys);
	void DeleteSystem(SYSTEMID ID);
	void ResetSystem(SYSTEMID ID);

private:
	std::list<System*> SystemList;
};