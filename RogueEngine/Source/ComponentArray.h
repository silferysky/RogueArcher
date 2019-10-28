#pragma once
#include <unordered_map>
#include <cassert>

#include "EntityManager.h"

namespace Rogue
{
	class BaseComponentArray
	{
	public:
		virtual ~BaseComponentArray() = default;
		virtual void clone(Entity, Entity) = 0;
		virtual void EntityDestroyed(Entity entity) = 0;
	};


	template<typename T>
	class ComponentArray : public BaseComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			RE_ASSERT(REEntityToIndexMap.find(entity) == REEntityToIndexMap.end(), "Component added to same entity more than once.");
			// Put new entry at end and update the maps
			size_t newIndex = RESize;
			REEntityToIndexMap[entity] = newIndex;
			REIndexToEntityMap[newIndex] = entity;
			REComponentArray[newIndex] = component;
			++RESize;
		}

		void RemoveData(Entity entity)
		{
			RE_ASSERT(REEntityToIndexMap.find(entity) != REEntityToIndexMap.end(), "Removing non-existent component.");
			if (!RESize)
				return;
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
			RE_ASSERT(REEntityToIndexMap.find(entity) != REEntityToIndexMap.end(), "Retrieving non-existent component.");
			return REComponentArray[REEntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			if (REEntityToIndexMap.find(entity) != REEntityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}
		
		void clone(Entity existingEntity, Entity toClone)
		{
			if (ComponentExists(existingEntity))
			{
				InsertData(toClone, T{ GetData(existingEntity) });
			}
		}

		Entity GetEntityFromIndex(size_t index) const
		{
			return REIndexToEntityMap[index];
		}

		size_t GetIndexFromEntity(Entity entity) const
		{
			return REEntityToIndexMap[entity];
		}

	private:

		inline bool ComponentExists(Entity entity)
		{
			return REEntityToIndexMap.find(entity) != REEntityToIndexMap.end();
		}

		std::array<T, MAX_ENTITIES> REComponentArray;

		std::unordered_map<Entity, size_t> REEntityToIndexMap;

		std::unordered_map<size_t, Entity> REIndexToEntityMap;

		size_t RESize;
	};
}