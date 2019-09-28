#pragma once
#include <set>
#include <unordered_map>
#include "EntityManager.h"
#include "ComponentArray.h"
class System
{
public:
	std::set<Entity> m_entities;
	virtual void init() = 0;
	virtual void update() = 0;
	virtual ~System() = default;
};

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>();
		RESystems.insert({ typeName, system });
		std::cout << "System Registered" << std::endl;
		return system;
	}

	void initSystems()
	{
		for (auto system : RESystems)
		{
			system.second->init();
		}
	}

	void updateSystems()
	{
		for (auto system : RESystems)
		{
			system.second->update();
		}
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		// Set the signature for this system
		RESignatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity)
	{
		// Erase a destroyed entity from all system lists
		for (auto const& pair : RESystems)
		{
			auto const& system = pair.second;

			system->m_entities.erase(entity);
			std::cout << "Entities Removed from system" << std::endl;
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : RESystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = RESignatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->m_entities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->m_entities.erase(entity);
			}
		}
	}

private:

	std::unordered_map<const char*, Signature> RESignatures{};

	std::unordered_map<const char*, std::shared_ptr<System>> RESystems{};
};