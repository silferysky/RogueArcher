#pragma once
#include <unordered_map>
#include "EntityManager.h"
#include "ComponentArray.h"
#include "ComponentList.h"

class ComponentManager
{
public:
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		// Add this component type to the component type map
		REComponentTypes.insert({ typeName, RENextComponentType });
		
		// Create a ComponentArray pointer and add it to the component arrays map
		REComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		// Increment the value so that the next component registered will be different
		++RENextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		return REComponentTypes[typeName];
	}

	ComponentType GetComponentType(const char* typeName)
	{
		return REComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		RE_CORE_INFO("components added to system\n");
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
		for (auto const& pair : REComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
			RE_CORE_INFO("Components Removed\n");
		}
	}

	size_t Size() const
	{
		return RENextComponentType;
	}

private:

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();
		return std::static_pointer_cast<ComponentArray<T>>(REComponentArrays[typeName]);
	}

	ComponentType RENextComponentType{};

	std::unordered_map<const char*, ComponentType> REComponentTypes{};

	std::unordered_map<const char*, std::shared_ptr<BaseComponentArray>> REComponentArrays{};

};