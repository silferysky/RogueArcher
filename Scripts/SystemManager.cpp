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
	while (!SystemList.empty())
		SystemList.pop_front();
}

void SystemManager::ResetSystem(SYSTEMID ID)
{
}

System* SystemManager::GetSystem(SYSTEMID ID)
{
	std::list<System*>::iterator sysIt = SystemList.begin();
	int count = 0;
	while (count < (int)ID)
	{
		++sysIt;
	}
	return *sysIt;
}
