#pragma once
#include <set>
#include <cassert>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "EntityManager.h"
#include "ComponentArray.h"
#include "Logger.h"
#include "Event.h"
#include "SystemList.h"
#include "BaseSystem.h"

class SystemManager
{
	template <typename T>
	static constexpr std::type_index GetTypeIndex(const T&) { return std::type_index(typeid(T)); }
	
	template <typename T>
	static constexpr std::type_index GetTypeIndex() { return std::type_index(typeid(T)); }

public:
	template<typename T>
	void RegisterSystem()
	{
		std::type_index typeName = GetTypeIndex<T>();
		RE_ASSERT(RESystems.find(typeName) == RESystems.end(), "Registering system more than once.");

		// Insert the newly created system pointer and typename into the map.
		RESystems.insert({ typeName, std::make_shared<T>() });
	}

	template<typename T>
	std::shared_ptr<T> GetSystem()
	{
		std::type_index SystemName = GetTypeIndex<T>();
		
		auto i = RESystems.find(SystemName);
		if (i != RESystems.end())
		{
			return std::dynamic_pointer_cast<T>(i->second);
		}
	}

	void InitSystems()
	{
		for (auto system : RESystems)
		{
			system.second->init();
		}
	}

	void UpdateSystems()
	{
		for (auto system : RESystems)
		{
			system.second->update();
		}
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		std::type_index typeName = GetTypeIndex<T>();

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
		std::stringstream out;

		// Notify each system that an entity's signature changed
		for (auto const& pair : RESystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = RESignatures[type];


			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				// Signature matched.
				CLEARSTRING(out);
				out << "Entity " << entity << "'s signature matches.";
				RE_CORE_INFO(out.str());
				
				if (system->m_entities.insert(entity).second)
				{
					CLEARSTRING(out);
					out << "Entity added to " << type.name() << ".";
					RE_CORE_INFO(out.str());
				}
				else
				{
					CLEARSTRING(out);
					out << "Entity already exists in " << type.name()<< ".";
					RE_CORE_INFO(out.str());
				}
			}
			// Entity signature does not match system signature - erase from set/don't add
			else
			{
				CLEARSTRING(out);
				out << "Signature " << entity << " does not match " << type.name() << ".";
				RE_CORE_INFO(out.str());

				if (system->m_entities.find(entity) != system->m_entities.end()) // If entity exists
				{
					RE_CORE_INFO("Entity removed to system");
				}
				else
				{
					RE_CORE_INFO("Entity not added to system");
				}
				system->m_entities.erase(entity);
			}
		}
	}

private:
	std::unordered_map<std::type_index, Signature> RESignatures{};
	std::unordered_map<std::type_index, std::shared_ptr<System>> RESystems{};
};