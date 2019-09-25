#pragma once
#include <list>					//std::list
#include <memory>				//std::unique_ptr
#include "RogueEngine.h"
#include "MemoryType.h"

#define MEM_SPACE 1024 * 1024
#define MAX_CHUNK_SIZE 1024

struct MemChunk
{
	int* chunkStart;
	size_t size;

	MemChunk()
		:chunkStart(nullptr), size(0) {}

	MemChunk(int* start, size_t sz)
		:chunkStart(start), size(sz) {}

	bool operator==(MemChunk chunk)
	{
		if (chunkStart == chunk.chunkStart)
			return true;
		return false;
	}
};

class MemoryManager :
	public BaseSystem
{
public:
	MemoryManager();
	~MemoryManager();

	static void*		Allocate(const size_t size);
	static void			Deallocate(const int* ptr, const size_t size);
	static bool			FindSpareChunk(const size_t size);
	static MemChunk		FindUsedChunk(int* ptr);
	static bool			CombineChunks();

private:

	static int* MemoryStart;
	static int* MemoryCurrent;
	static std::list<MemChunk> MemorySpare;
	static std::list<MemChunk> MemoryUsed;
};

//For Generic overload new/delete
void* operator new(size_t space, MemoryType mem)
{
	void* ptr = MemoryManager::Allocate(space);
	return ptr;
}

void* operator new[](size_t space)
{
	void* ptr = MemoryManager::Allocate(space);
	return ptr;
}

void operator delete(void* ptr)
{
	if (ptr != nullptr)
	{
		MemChunk toDeallocate = MemoryManager::FindUsedChunk((int*)ptr);
		MemoryManager::Deallocate(toDeallocate.chunkStart, toDeallocate.size);
	}
}

void operator delete[](void* ptr)
{
	if (ptr != nullptr)
	{
		MemChunk toDeallocate = MemoryManager::FindUsedChunk((int*)ptr);
		MemoryManager::Deallocate(toDeallocate.chunkStart, toDeallocate.size);
	}
}