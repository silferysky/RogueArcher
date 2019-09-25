#include "MemoryManager.h"

/***************Definition/Initialization of values*****************/
int* MemoryManager::MemoryStart;
int* MemoryManager::MemoryCurrent;
std::list<MemChunk> MemoryManager::MemorySpare; 
std::list<MemChunk> MemoryManager::MemoryUsed; 
/*******************************************************************/

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

	if (SpaceUsed > MEM_SPACE)
	{
		//Attempt to save space by combining chunks
		while(CombineChunks());
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
		RE_CORE_WARN("MAXED OUT MEMORY");

	/***********************************************************
					Hierarchy of Memory Manager

	1) Old Memory spaces with perfect size
	2) Old Memory spaces that has empty spaces after allocation
	3) Current memory slot
	***********************************************************/

	//Set default memory slot, assign to MemoryCurrent
	MemChunk chunkToReturn;
	chunkToReturn.chunkStart = MemoryCurrent;
	chunkToReturn.size = MAX_CHUNK_SIZE;

	//Search through list to see if an available chunk is found
	//If an available chunk is found, MemoryCurrent and size will be updated to match
	for (auto const& i : MemorySpare)
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
		MemoryCurrent += size;
		MemoryUsed.push_back(MemChunk(MemoryCurrent, size));
	}
	else
	{
		//If perfect match is found, remove chunk from list
		//If perfect match not found, create a smaller chunk from the remains of the current chunk
		//Note that chunkToReturn size will always be updated (Not MAX_CHUNK_SIZE)

		//Removing chunk from memory, adding to MemoryUsed. Using size instead of chunkToReturn size
		//since size might not be identical
		MemorySpare.remove(chunkToReturn);
		MemoryUsed.push_front(MemChunk(chunkToReturn.chunkStart, size));

		if (chunkToReturn.size != size)
			MemorySpare.push_front(MemChunk(chunkToReturn.chunkStart + size, chunkToReturn.size - size));
	}

	return chunkToReturn.chunkStart;
	
}

void MemoryManager::Deallocate(const int* ptr, const size_t size)
{
	MemChunk newChunk;
	newChunk.chunkStart = (int*)ptr;
	newChunk.size = size;

	MemorySpare.push_front(newChunk);
}

bool MemoryManager::FindSpareChunk(const size_t size)
{
	//Search through all chunks
	for (auto const &i : MemorySpare)
	{
		//If inappropriate size, ignore. Otherwise break and return true
		if (size > i.size)
			continue;
		return true;
	}

	//If all chunks have been iterated through, it means no chunks found
	return false;
}

MemChunk MemoryManager::FindUsedChunk(int* ptr)
{
	for (auto const &i : MemoryUsed)
	{
		//Look for the correct position
		if (i.chunkStart == ptr)
			return i;
	}

	//If none found, ignore and return empty chunk
	return MemChunk();
}

bool MemoryManager::CombineChunks()
{
	//Boolean to return, if chunks managed to combine
	bool chunksCombined = false;

	//For each chunk in Memory
	for (auto &i : MemorySpare)
	{
		//Check through all other chunks
		for (auto &j : MemorySpare)
		{
			//If chunk is the same, or before it, skip
			if (i == j)
				continue;

			//Check only future chunks 
			if (i.chunkStart + i.size == j.chunkStart)
			{
				chunksCombined = true;
				i.size += j.size;
				MemorySpare.remove(j);
			}
		}
	}

	return chunksCombined;
}