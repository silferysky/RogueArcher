#include "MemoryManager.h"



MemoryManager::MemoryManager()
{
	RE_INFO("INIT MEMORY MANAGER");
}


MemoryManager::~MemoryManager()
{
	RE_INFO("CLOSE MEMORY MANAGER");
	//std::for_each(SysList.begin(), SysList.end(), DeleteSharedPtrTo<BaseSystem>);
	//std::for_each(EntList.begin(), EntList.end(), DeleteSharedPtrTo<BaseEntity>);
	//std::for_each(CmpList.begin(), CmpList.end(), DeleteSharedPtrTo<BaseComponent>);
}

void MemoryManager::AddIntoMemory(BaseSystem &sys)
{
	std::shared_ptr<BaseSystem> ptr = std::make_shared<BaseSystem>(sys);
	SysList.push_back(ptr);
}

void MemoryManager::AddIntoMemory(BaseEntity &ent)
{
	std::shared_ptr<BaseEntity> ptr = std::make_shared<BaseEntity>(ent);
	EntList.push_back(ptr);
}

void MemoryManager::AddIntoMemory(BaseComponent &cmp)
{
	std::shared_ptr<BaseComponent> ptr = std::make_shared<BaseComponent>(cmp);
	CmpList.push_back(ptr);
}

void MemoryManager::RemoveFromMemory(BaseSystem &sys)
{
	if (!SysList.size())
		return;

	std::shared_ptr<BaseSystem> ptr(&sys);
	auto it = std::find(SysList.begin(), SysList.end(),	ptr);

	//If item is found, check if unique. Only remove if unique
	if (it != SysList.end())
		if (ptr.unique())
			SysList.erase(it);
}

void MemoryManager::RemoveFromMemory(BaseEntity &ent)
{
	if (!EntList.size())
		return;

	std::shared_ptr<BaseEntity> ptr(&ent);
	auto it = std::find(EntList.begin(), EntList.end(), ptr);

	//If item is found, check if unique. Only remove if unique
	if (it != EntList.end())
		if(ptr.unique())
			EntList.erase(it);
}

void MemoryManager::RemoveFromMemory(BaseComponent &cmp)
{
	if (!CmpList.size())
		return;

	std::shared_ptr<BaseComponent> ptr(&cmp);
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

std::vector<std::shared_ptr<BaseSystem>> MemoryManager::GetSysList()
{
	return SysList;
}

std::vector<std::shared_ptr<BaseEntity>> MemoryManager::GetEntList()
{
	return EntList;
}

std::vector<std::shared_ptr<BaseComponent>> MemoryManager::GetCmpList()
{
	return CmpList;
}

