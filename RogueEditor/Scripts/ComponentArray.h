#pragma once
#include "EntityManager.h"
#include <unordered_map>
class BaseComponentArray
{
public:
	virtual ~BaseComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T>
class ComponentArray : public BaseComponentArray
{
public:
	void InsertData(Entity entity, T component)
	{
		// Put new entry at end and update the maps
		size_t newIndex = RESize;
		REEntityToIndexMap[entity] = newIndex;
		REIndexToEntityMap[newIndex] = entity;
		REComponentArray[newIndex] = component;
		++RESize;
	}

	void RemoveData(Entity entity)
	{
		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = REEntityToIndexMap[entity];
		size_t indexOfLastElement = RESize - 1;
		REComponentArray[indexOfRemovedEntity] = REComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = REIndexToEntityMap[indexOfLastElement];
		REEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		REIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		REEntityToIndexMap.erase(entity);
		REIndexToEntityMap.erase(indexOfLastElement);

		--RESize;
	}

	T& GetData(Entity entity)
	{
		auto idx = REEntityToIndexMap[entity];
		if (idx >= 0 && idx < MAX_ENTITIES)
			return REComponentArray[idx];
		else
			return REComponentArray[0];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (REEntityToIndexMap.find(entity) != REEntityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}

private:

	std::array<T, MAX_ENTITIES> REComponentArray;

	std::unordered_map<Entity, size_t> REEntityToIndexMap;

	std::unordered_map<size_t, Entity> REIndexToEntityMap;

	size_t RESize;
};