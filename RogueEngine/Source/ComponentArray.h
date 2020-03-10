/* Start Header ************************************************************************/
/*!
\file           ComponentArray.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ComponentArray

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "EntityManager.h"

namespace Rogue
{
	class BaseComponentArray
	{
	public:
		virtual ~BaseComponentArray() = default;
		virtual void Clone(Entity, Entity) = 0;
		virtual void EntityDestroyed(Entity entity) = 0;
	};


	template<typename TComponent>
	class ComponentArray : public BaseComponentArray
	{
	public:
		ComponentArray() :
			m_size{ 0 }
		{}

		void InsertData(Entity entity, const TComponent& component)
		{
			RE_ASSERT(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end(), "Component added to same entity more than once.");

			// Put new entry at end and update the maps
			size_t newIndex = m_size;
			m_entityToIndexMap[entity] = newIndex;
			m_indexToEntityMap[newIndex] = entity;
			m_componentArray[newIndex] = component;
			++m_size;
		}

		void RemoveData(Entity entity)
		{

			RE_ASSERT(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end(), "Removing non-existent component.");
			
			if (!m_size)
				return;
			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
			size_t indexOfLastElement = m_size - 1;
			m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
			m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_entityToIndexMap.erase(entity);
			m_indexToEntityMap.erase(indexOfLastElement);

			--m_size;
		}

		TComponent& GetData(Entity entity)
		{
			RE_ASSERT(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end(), "Retrieving non-existent component.");
			return m_componentArray[m_entityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}
		
		void Clone(Entity existingEntity, Entity toClone) override
		{
			if (ComponentExists(existingEntity))
			{
				InsertData( toClone, GetData(existingEntity) );
			}
		}

		/*Entity GetEntityFromIndex(size_t index) const
		{
			return REIndexToEntityMap[index];
		}

		size_t GetIndexFromEntity(Entity entity) const
		{
			return REEntityToIndexMap[entity];
		}*/

	private:

		inline bool ComponentExists(Entity entity)
		{
			return m_entityToIndexMap.find(entity) != m_entityToIndexMap.end();
		}

		std::array<TComponent, MAX_ENTITIES> m_componentArray;

		std::unordered_map<Entity, size_t> m_entityToIndexMap;

		std::unordered_map<size_t, Entity> m_indexToEntityMap;

		size_t m_size;
	};
}