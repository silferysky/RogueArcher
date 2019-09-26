#pragma once
#include "EntityManager.h"
#include <unordered_map>
class BaseComponentArray
{
public:
	virtual ~BaseComponentArray() = default;
	virtual void EntityDestroyed(std::uint32_t entity) = 0;
};


template<typename T>
class ComponentArray : public BaseComponentArray
{
public:
	void InsertData(std::uint32_t entity, T component)
	{
		// Put new entry at end and update the maps
		size_t newIndex = RESize;
		REEntityToIndexMap[entity] = newIndex;
		REIndexToEntityMap[newIndex] = entity;
		REComponentArray[newIndex] = component;
		++RESize;
	}

	void RemoveData(std::uint32_t entity)
	{
		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = REEntityToIndexMap[entity];
		size_t indexOfLastElement = RESize - 1;
		REComponentArray[indexOfRemovedEntity] = REComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		std::uint32_t entityOfLastElement = REIndexToEntityMap[indexOfLastElement];
		REEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		REIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		REEntityToIndexMap.erase(entity);
		REIndexToEntityMap.erase(indexOfLastElement);

		--RESize;
	}

	T& GetData(std::uint32_t entity)
	{
		return REComponentArray[REEntityToIndexMap[entity]];
	}

	void EntityDestroyed(std::uint32_t entity) override
	{
		if (REEntityToIndexMap.find(entity) != REEntityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}

private:

	std::array<T, MAX_ENTITIES> REComponentArray;

	std::unordered_map<std::uint32_t, size_t> REEntityToIndexMap;

	std::unordered_map<size_t, std::uint32_t> REIndexToEntityMap;

	size_t RESize;
};