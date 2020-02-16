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
			//const char* typeName = typeid(T).name();
			//RE_ASSERT(m_componentTypes.find(typeName) != m_componentTypes.end(), "Component not registered before use.");
			return TypeID<TComponent>::value();
		}

		template<typename TComponent>
		void RegisterComponent()
		{
			//RE_ASSERT(m_componentTypes.find(typeName) == m_componentTypes.end(), "Registering component type more than once.");

			// Add this component type to the component type map
			//m_componentTypes.insert({ typeName, m_nextComponentType });

			//std::stringstream out;
			//out.clear();
			//out.str("");
			//out << "Creating " << typeName << "s...";
			//RE_CORE_INFO(out.str());

			// Create a ComponentArray pointer and add it to the component arrays map
			m_componentArrays.insert({ GetComponentType<TComponent>(), std::make_shared<ComponentArray<TComponent>>() });
			
			//out.clear();
			//out.str("");
			//out << "Array of " << MAX_ENTITIES << " " << typeName << "s created!";
			//RE_CORE_INFO(out.str());

			// Increment the value so that the next component registered will be different
			//++m_nextComponentType;
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			//std::stringstream out;
			//out << "Added " << typeid(T).name() << " to Entity " << entity;
			//RE_CORE_INFO(out.str());
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const& pair : m_componentArrays)
			{
				auto const& component = pair.second;

				component->EntityDestroyed(entity);
				//RE_CORE_INFO("Components Removed\n");
			}
		}
		
		void clone(Entity existingEntity, Entity toClone)
		{
			for(auto i = m_componentArrays.begin(); i != m_componentArrays.cend(); i++)
				i->second->clone(existingEntity, toClone);
		}

	private:

		template<typename TComponent>
		std::shared_ptr<ComponentArray<TComponent>> GetComponentArray()
		{

			//const char* typeName = typeid(T).name();
			//RE_ASSERT(m_componentTypes.find(typeName) != m_componentTypes.end(), "Component not registered before use.");
			//return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
			
			return std::static_pointer_cast<ComponentArray<TComponent>>( m_componentArrays[GetComponentType<TComponent>()] );
		}

		//ComponentType m_nextComponentType{};

		//std::unordered_map<const char*, ComponentType> m_componentTypes{};

		std::unordered_map<ComponentType, std::shared_ptr<BaseComponentArray>> m_componentArrays{};

	};
}