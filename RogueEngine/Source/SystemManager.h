#pragma once
#include <set>
#include <cassert>
#include <unordered_map>

#include "EntityManager.h"
#include "ComponentArray.h"
#include "Logger.h"
#include "Event.h"
#include "SystemList.h"
#include "BaseSystem.h"

class SystemManager
{
public:
	template<typename T>
	void RegisterSystem()
	{
		const char* typeName = typeid(T).name();
		RE_ASSERT(RESystems.find(typeName) == RESystems.end(), "Registering system more than once.");

		// Insert the newly created system pointer and typename into the map.
		RESystems.insert({ typeName, std::make_shared<T>() });

		std::stringstream loggerStr;
		loggerStr << typeName << " registered!";
		RE_CORE_INFO(loggerStr.str());
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
					out << "Entity added to " << type << ".";
					RE_CORE_INFO(out.str());
				}
				else
				{
					CLEARSTRING(out);
					out << "Entity already exists in " << type << ".";
					RE_CORE_INFO(out.str());
				}
			}
			// Entity signature does not match system signature - erase from set/don't add
			else
			{
				CLEARSTRING(out);
				out << "Signature " << entity << " does not match " << type << ".";
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
	std::unordered_map<const char*, Signature> RESignatures{};
	std::unordered_map<const char*, std::shared_ptr<System>> RESystems{};
};