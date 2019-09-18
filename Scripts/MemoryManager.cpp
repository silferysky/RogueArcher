#include "MemoryManager.h"

/***************Definition/Initialization of values*****************/
std::map<const void*, const size_t> MemoryManager::SystemMap;
std::map<const void*, const size_t> MemoryManager::EntityMap;
std::map<const void*, const size_t> MemoryManager::ComponentMap;
size_t MemoryManager::SystemAlloc;
size_t MemoryManager::EntityAlloc;
size_t MemoryManager::ComponentAlloc;
size_t MemoryManager::SystemAllocBytes;
size_t MemoryManager::EntityAllocBytes;
size_t MemoryManager::ComponentAllocBytes;
/********************************************************************/

MemoryManager::MemoryManager()
{
	RE_INFO("INIT MEMORY MANAGER");

	//Resetting Systems
	SystemAlloc = 0;
	EntityAlloc = 0;
	ComponentAlloc = 0;
	SystemAllocBytes = 0;
	EntityAllocBytes = 0;
	ComponentAllocBytes = 0;

	//Resetting Maps
	SystemMap.clear();
	EntityMap.clear();
	ComponentMap.clear();
}

MemoryManager::~MemoryManager()
{
	RE_INFO("CLOSE MEMORY MANAGER");
	//std::for_each(SysList.begin(), SysList.end(), DeleteSharedPtrTo<BaseSystem>);
	//std::for_each(EntList.begin(), EntList.end(), DeleteSharedPtrTo<BaseEntity>);
	//std::for_each(CmpList.begin(), CmpList.end(), DeleteSharedPtrTo<BaseComponent>);
}

void MemoryManager::add(const void* ptr, const size_t size, MemoryType memType)
{
	switch (memType)
	{
	case MemorySystem:
		RE_CORE_INFO("ADDING SYSTEM MEMORY TYPE");
		++SystemAlloc;
		SystemAllocBytes += size;
		SystemMap.insert(std::pair<const void*, const size_t>(ptr, size));
		break;
	case MemoryEntity:
		RE_CORE_INFO("ADDING ENTITY MEMORY TYPE");
		++EntityAlloc;
		EntityAllocBytes += size;
		EntityMap.insert(std::pair<const void*, const size_t>(ptr, size));
		break;
	case MemoryComponent:
		RE_CORE_INFO("ADDING COMPONENT MEMORY TYPE");
		++ComponentAlloc;
		ComponentAllocBytes += size;
		ComponentMap.insert(std::pair<const void*, const size_t>(ptr, size));
		break;
	default:
		RE_CORE_WARN("ADDING NONDEFINED MEMORY TYPE");
		break;
	}
}

void MemoryManager::remove(const void* ptr, MemoryType memType)
{
	//TODO: Find a way to mitigate the use of MemoryType to prevent accidental deletes
	size_t size;
	switch (memType)
	{
	case MemorySystem:
		RE_CORE_INFO("REMOVING SYSTEM MEMORY TYPE");
		size = SystemMap[ptr];
		--SystemAlloc;
		SystemAllocBytes -= size;
		SystemMap.erase(ptr);
		break;
	case MemoryEntity:
		RE_CORE_INFO("REMOVING ENTITY MEMORY TYPE");
		size = EntityMap[ptr];
		--EntityAlloc;
		EntityAllocBytes -= size;
		EntityMap.erase(ptr);
		break;
	case MemoryComponent:
		RE_CORE_INFO("REMOVING COMPONENT MEMORY TYPE");
		size = ComponentMap[ptr];
		--ComponentAlloc;
		ComponentAllocBytes -= size;
		ComponentMap.erase(ptr);
		break;
	default:
		RE_CORE_WARN("REMOVING NONDEFINED MEMORY TYPE");
		break;
	}
}