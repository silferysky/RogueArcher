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

	bool operator==(MemChunk chunk); 
};

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();

	static MemoryManager& instance();

	static void*		Allocate(const size_t size);
	static void			Deallocate(int* ptr, const size_t size);
	static bool			FindSpareChunk(const size_t size);
	static MemChunk*	FindUsedChunk(int* ptr);
	static bool			CombineChunks();

	//For Generic overload new/delete
	void* operator new(size_t space)
	{
		void* ptr = MemoryManager::instance().Allocate(space);
		return ptr;
	}

	void* operator new[](size_t space)
	{
		void* ptr = MemoryManager::instance().Allocate(space);
		return ptr;
	}

	void operator delete(void* ptr)
	{
		if (ptr != nullptr)
		{
			MemChunk* toDeallocate = MemoryManager::instance().FindUsedChunk((int*)ptr);
			MemoryManager::Deallocate(toDeallocate->chunkStart, toDeallocate->size);
		}
	}

	void operator delete[](void* ptr)
	{
		if (ptr != nullptr)
		{
			MemChunk* toDeallocate = MemoryManager::instance().FindUsedChunk((int*)ptr);
			MemoryManager::Deallocate(toDeallocate->chunkStart, toDeallocate->size);
		}
	}

private:

	static int* MemoryStart;
	static int* MemoryCurrent;
	static std::vector<MemChunk> MemorySpare;
	static std::vector<MemChunk> MemoryUsed;
};