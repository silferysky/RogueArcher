/* Start Header ************************************************************************/
/*!
\file           MemoryManager.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for MemoryManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <vector>

#define MEM_SPACE 1024 * 1024
#define MAX_CHUNK_SIZE 1024

namespace Rogue
{
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

		static void* Allocate(const size_t size);
		static void			Deallocate(int* ptr, const size_t size);
		static bool			FindSpareChunk(const size_t size);
		static MemChunk* FindUsedChunk(int* ptr);
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
}