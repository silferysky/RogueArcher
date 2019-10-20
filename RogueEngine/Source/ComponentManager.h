#pragma once
#include <unordered_map>
#include <cassert>

#include "EntityManager.h"
#include "ComponentArray.h"
#include "ComponentList.h"
#include "Logger.h"

class ComponentManager
{
public:
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();
		RE_ASSERT(REComponentTypes.find(typeName) == REComponentTypes.end(), "Registering component type more than once.");

		// Add this component type to the component type map
		REComponentTypes.insert({ typeName, RENextComponentType });

		std::stringstream out;
		out.clear();
		out.str("");
		out << "Creating " << typeName << "s...";
		RE_CORE_INFO(out.str());

		// Create a ComponentArray pointer and add it to the component arrays map
		REComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });


		out.clear();
		out.str("");
		out << "Array of " << MAX_ENTITIES << " " << typeName << "s created!";
		RE_CORE_INFO(out.str());

		// Increment the value so that the next component registered will be different
		++RENextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();
		RE_ASSERT(REComponentTypes.find(typeName) != REComponentTypes.end(), "Component not registered before use.");
		return REComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		std::stringstream out;
		out << "Added " << typeid(T).name() << " to Entity " << entity;
		RE_CORE_INFO(out.str());
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
		RE_ASSERT(REComponentTypes.find(typeName) != REComponentTypes.end(), "Component not registered before use.");
		return std::static_pointer_cast<ComponentArray<T>>(REComponentArrays[typeName]);
	}

	ComponentType RENextComponentType{};

	std::unordered_map<const char*, ComponentType> REComponentTypes{};

	std::unordered_map<const char*, std::shared_ptr<BaseComponentArray>> REComponentArrays{};

};