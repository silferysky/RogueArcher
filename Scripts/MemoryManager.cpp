#include "MemoryManager.h"



MemoryManager::MemoryManager()
{
	RE_INFO("INIT MEMORY MANAGER");
}


MemoryManager::~MemoryManager()
{
	RE_INFO("CLOSE MEMORY MANAGER");
	//std::for_each(SysList.begin(), SysList.end(), DeleteSharedPtrTo<System>);
	//std::for_each(EntList.begin(), EntList.end(), DeleteSharedPtrTo<Entity>);
	//std::for_each(CmpList.begin(), CmpList.end(), DeleteSharedPtrTo<Component>);
}

void MemoryManager::AddIntoMemory(System &sys)
{
	std::shared_ptr<System> ptr = std::make_shared<System>(sys);
	SysList.push_back(ptr);
}

void MemoryManager::AddIntoMemory(Entity &ent)
{
	std::shared_ptr<Entity> ptr = std::make_shared<Entity>(ent);
	EntList.push_back(ptr);
}

void MemoryManager::AddIntoMemory(Component &cmp)
{
	std::shared_ptr<Component> ptr = std::make_shared<Component>(cmp);
	CmpList.push_back(ptr);
}

void MemoryManager::RemoveFromMemory(System &sys)
{
	if (!SysList.size())
		return;

	std::shared_ptr<System> ptr(&sys);
	auto it = std::find(SysList.begin(), SysList.end(),	ptr);

	//If item is found, check if unique. Only remove if unique
	if (it != SysList.end())
		if (ptr.unique())
			SysList.erase(it);
}

void MemoryManager::RemoveFromMemory(Entity &ent)
{
	if (!EntList.size())
		return;

	std::shared_ptr<Entity> ptr(&ent);
	auto it = std::find(EntList.begin(), EntList.end(), ptr);

	//If item is found, check if unique. Only remove if unique
	if (it != EntList.end())
		if(ptr.unique())
			EntList.erase(it);
}

void MemoryManager::RemoveFromMemory(Component &cmp)
{
	if (!CmpList.size())
		return;

	std::shared_ptr<Component> ptr(&cmp);
	auto it = std::find(CmpList.begin(), CmpList.end(), ptr);

	//If item is found, check if unique. Only remove if unique
	if (it != CmpList.end())
		if(ptr.unique())
			CmpList.erase(it);
}

size_t MemoryManager::SysListCount()
{
	return SysList.size();
}

size_t MemoryManager::EntListCount()
{
	return EntList.size();
}

size_t MemoryManager::CmpListCount()
{
	return CmpList.size();
}

std::vector<std::shared_ptr<System>> MemoryManager::GetSysList()
{
	return SysList;
}

std::vector<std::shared_ptr<Entity>> MemoryManager::GetEntList()
{
	return EntList;
}

std::vector<std::shared_ptr<Component>> MemoryManager::GetCmpList()
{
	return CmpList;
}

