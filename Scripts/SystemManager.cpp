#include "SystemManager.h"



SystemManager::SystemManager()
{
	AddSystem(new MemoryManager());
	AddSystem(new InputManager());
	AddSystem(new EventManager());
	//AddSystem(new AudioManager());
}

SystemManager::~SystemManager()
{
	SystemList.clear();
}

void SystemManager::AddSystem(System* Sys)
{
	SystemList.push_back(Sys);
}

void SystemManager::DeleteSystem(SYSTEMID id)
{
	System* sys;
	//SystemList.erase(SystemList.ge]);
}