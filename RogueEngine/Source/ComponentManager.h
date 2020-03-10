/* Start Header ************************************************************************/
/*!
\file           ComponentManager.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ComponentManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "EntityManager.h"
#include "ComponentArray.h"
#include "ComponentList.h"
#include "ComponentIndex.h"
#include "Logger.h"

namespace Rogue
{
	class ComponentManager
	{
	public:
		template<typename TComponent>
		ComponentType GetComponentType()
		{
			return TypeID<TComponent>::value();
		}

		template<typename TComponent>
		void RegisterComponent()
		{
			// Create a ComponentArray pointer and add it to the component arrays vector
			if (m_componentArrays.size() < GetComponentType<TComponent>() + 1L)
				m_componentArrays.resize(GetComponentType<TComponent>() + 1L);

			m_componentArrays[GetComponentType<TComponent>()] = std::make_shared<ComponentArray<TComponent>>();
		}

		template<typename TComponent>
		void AddComponent(Entity entity, TComponent component)
		{
			GetComponentArray<TComponent>()->InsertData( entity, component );
		}

		template<typename TComponent>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<TComponent>()->RemoveData(entity);
		}

		template<typename TComponent>
		TComponent& GetComponent(Entity entity)
		{
			return GetComponentArray<TComponent>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (const std::shared_ptr<BaseComponentArray>& pComponentArray : m_componentArrays)
			{
				pComponentArray->EntityDestroyed(entity);
			}
		}
		
		void Clone(Entity existingEntity, Entity toClone)
		{
			for(const std::shared_ptr<BaseComponentArray>& pComponentArray : m_componentArrays)
				pComponentArray->Clone(existingEntity, toClone);
		}

	private:
		template<typename TComponent>
		std::shared_ptr<ComponentArray<TComponent>> GetComponentArray()
		{
			return std::static_pointer_cast<ComponentArray<TComponent>>( m_componentArrays[GetComponentType<TComponent>()] );
		}

		std::vector<std::shared_ptr<BaseComponentArray>> m_componentArrays;
	};
}