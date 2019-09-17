#pragma once
#include <map>			//std::map
#include <algorithm>	//std::find
#include <iterator>		//std::iterator
#include <memory>		//std::shared_ptr
#include <cstdlib>		//std::pair
#include "RogueEngine.h"
#include "MemoryType.h"

static class MemoryManager :
	public BaseSystem
{
public:
	MemoryManager();
	~MemoryManager();

	static void add(const void* ptr, const size_t size, MemoryType memType);
	static void remove(const void* ptr, MemoryType memType);

private:

	static std::map<const void*, const size_t> SystemMap;
	static std::map<const void*, const size_t> EntityMap;
	static std::map<const void*, const size_t> ComponentMap;

	static size_t SystemAlloc;
	static size_t EntityAlloc;
	static size_t ComponentAlloc;

	static size_t SystemAllocBytes;
	static size_t EntityAllocBytes;
	static size_t ComponentAllocBytes;
};

//For Generic overload new/delete
void* operator new(size_t space, MemoryType mem)
{
	void* ptr = malloc(space);
	MemoryManager::add(ptr, space, mem);

	return ptr;
}

void* operator new[](size_t space, MemoryType mem)
{
	void* ptr = malloc(space);
	MemoryManager::add(ptr, space, mem);

	return ptr;
}

void operator delete(void* ptr, MemoryType mem)
{
	MemoryManager::remove(ptr, mem);
	free(ptr);
}

void operator delete[](void* ptr, MemoryType mem)
{
	MemoryManager::remove(ptr, mem);
	free(ptr);
}