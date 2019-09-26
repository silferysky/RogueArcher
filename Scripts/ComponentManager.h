#pragma once
#include <unordered_map>
#include "EntityManager.h"
#include "ComponentArray.h"

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
	std::uint8_t GetComponentType()
	{
		const char* typeName = typeid(T).name();

		return REComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(std::uint8_t entity, T component)
	{
		std::cout<<"components added to system"<<std::endl;
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(std::uint8_t entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(std::uint8_t entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(std::uint8_t entity)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : REComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
			std::cout << "Components Removed" << std::endl;
		}
	}

private:

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();
		return std::static_pointer_cast<ComponentArray<T>>(REComponentArrays[typeName]);
	}

	std::uint8_t RENextComponentType{};
	std::unordered_map<const char*, std::uint8_t> REComponentTypes{};
	std::unordered_map<const char*, std::shared_ptr<BaseComponentArray>> REComponentArrays{};

};