#pragma once
#include <set>
#include <unordered_map>
#include "EntityManager.h"
#include "ComponentArray.h"
#include "Logger.h"

class System
{
public:
	std::set<Entity> m_entities;
	System() = default;

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
		auto pSystem = std::make_shared<T>();
		RESystems.insert({ typeName, pSystem });
		RE_CORE_INFO("System registered!");
		return pSystem;
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
			RE_CORE_INFO("Entities Removed from system");
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		RE_CORE_INFO("Updating system's entity list");

		// Notify each system that an entity's signature changed
		for (auto const& pair : RESystems)
		{
			std::string typeName = pair.first;
			std::cout << "Looping through " << typeName << std::endl;

			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = RESignatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				// Signature matched.
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