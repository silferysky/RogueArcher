/* Start Header ************************************************************************/
/*!
\file           MemoryManager.cpp
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
#include "Precompiled.h"
#include "MemoryManager.h"
#include "Logger.h"
#include "MemoryType.h"

namespace Rogue
{
	///***************Definition/Initialization of values*****************/
	int* MemoryManager::MemoryStart;
	int* MemoryManager::MemoryCurrent;
	std::vector<MemChunk> MemoryManager::MemorySpare;
	std::vector<MemChunk> MemoryManager::MemoryUsed;
	/*******************************************************************/

	MemoryManager& MemoryManager::instance()
	{
		static MemoryManager instance;
		return instance;
	}

	MemoryManager::MemoryManager()
	{
		RE_INFO("INIT MEMORY MANAGER");

		MemoryStart = (int*)malloc(MEM_SPACE);
		MemoryCurrent = MemoryStart;
	}

	MemoryManager::~MemoryManager()
	{
		free(MemoryStart);

		RE_INFO("CLOSE MEMORY MANAGER");
	}

	void* MemoryManager::Allocate(const size_t size)
	{
		size_t SpaceUsed = MemoryCurrent - MemoryStart + size;
		bool AbortAllocate = false;

		//Check for SpaceUsed. 
		//If SpaceUsed exceeds, send an error message and aborts
		//If SpaceUsed is equal, sends a warning message only

		if (SpaceUsed > MEM_SPACE)
		{
			//Attempt to save space by combining chunks
			while (CombineChunks());

			//Check for empty spaces in List that fits the size
			AbortAllocate = true;
		}

		if (AbortAllocate)
		{
			//Error message if exceed bounds
			RE_CORE_ERROR("OUT OF MEMORY SPACE FOR ALLOCATION");
			return nullptr;
		}

		//Warning message if full
		if (SpaceUsed == MEM_SPACE)
		{
			RE_CORE_WARN("MAXED OUT MEMORY");
		}
		/***********************************************************
						Hierarchy of Memory Manager

		1) Old Memory spaces with perfect size
		2) Old Memory spaces that has empty spaces after allocation
		3) Current memory slot
		***********************************************************/

		//Set default memory slot, assign to MemoryCurrent
		//MemChunk chunk;
		MemChunk chunkToReturn(MemoryCurrent, MAX_CHUNK_SIZE);

		//Search through list to see if an available chunk is found
		//If an available chunk is found, MemoryCurrent and size will be updated to the chunk
		for (auto& i : MemorySpare)
		{
			//If perfect match, no need to iterate through, break out of loop
			if (i.size == size)
			{
				chunkToReturn = i;
				break;
			}
			//If not perfect match, but still larger, use smallest chunk found
			else if (i.size > size)
			{
				if (chunkToReturn.size > i.size)
					chunkToReturn = i;
			}
		}

		//Incrementing Memory
		//If no available chunks found, allocated memory goes to MemoryCurrent. Increment so no space is left
		if (chunkToReturn.chunkStart == MemoryCurrent)
		{
			MemChunk newChunk(MemoryCurrent, size);
			MemoryUsed.push_back(newChunk);
			MemoryCurrent += size;
		}
		else
		{
			//If perfect match is found, remove chunk from list
			//If perfect match not found, create a smaller chunk from the remains of the current chunk
			//Note that chunkToReturn size will always be updated (Not MAX_CHUNK_SIZE)

			//Removing chunk from memory, adding to MemoryUsed. Using size instead of chunkToReturn size
			//since size might not be identical
			for (auto it = MemorySpare.begin(); it != MemorySpare.end(); ++it)
			{
				if (it->chunkStart == chunkToReturn.chunkStart)
				{
					MemorySpare.erase(it);
					break;
				}
			}
			MemChunk newChunk(chunkToReturn.chunkStart, size);
			MemoryUsed.push_back(newChunk);

			//If the new chunk is not same size, it has leftovers, store leftover space inside
			if (chunkToReturn.size != size)
			{
				MemChunk spareChunk(chunkToReturn.chunkStart + size, chunkToReturn.size - size);
				MemorySpare.push_back(spareChunk);
			}
		}

		return chunkToReturn.chunkStart;

	}

	void MemoryManager::Deallocate(int* ptr, const size_t size)
	{
		if (ptr)
		{
			MemChunk newChunk;
			newChunk.chunkStart = (int*)ptr;
			newChunk.size = size;

			MemorySpare.push_back(newChunk);
		}
	}

	bool MemoryManager::FindSpareChunk(const size_t size)
	{
		//Search through all chunks
		for (auto& i : MemorySpare)
		{
			//If inappropriate size, ignore. Otherwise break and return true
			if (size > i.size)
				continue;
			return true;
		}

		//If all chunks have been iterated through, it means no chunks found
		return false;
	}

	MemChunk* MemoryManager::FindUsedChunk(int* ptr)
	{
		for (auto& i : MemoryUsed)
		{
			//Look for the correct position
			if (i.chunkStart == ptr)
				return &i;
		}

		//If none found, ignore and return nullptr
		return nullptr;
	}

	bool MemoryManager::CombineChunks()
	{
		//Boolean to return, if chunks managed to combine
		bool chunksCombined = false;
		int index_1 = 0;
		//For each chunk in Memory
		for (auto& i : MemorySpare)
		{
			int index_2 = 0;
			//Check through all other chunks
			for (auto& j : MemorySpare)
			{
				//If chunk is the same, or before it, skip
				if (index_1 >= index_2)
				{
					++index_2;
					continue;
				}

				//Check only future chunks 
				if (i.chunkStart + i.size == j.chunkStart)
				{
					chunksCombined = true;
					i.size += j.size;
					//MemorySpare.remove(j);
				}
				else
					++index_2;
			}

			++index_1;
		}

		return chunksCombined;
	}

	bool MemChunk::operator==(MemChunk rhs)
	{
		if (chunkStart == rhs.chunkStart)
			return true;
		return false;
	}
}