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

namespace Rogue
{
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
				system->m_entities.erase(entity);
			
				std::stringstream str;
				str << "Entity " << entity << " Removed from all systems";
				RE_CORE_INFO(str.str());

				break;
			}
		}
		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			std::stringstream out;

			// Notify each system that an entity's signature changed
			for (auto const& pair : RESystems)
			{
				std::type_index type = pair.first;
				std::shared_ptr<System> system = pair.second;
				std::unordered_map<std::type_index, Signature>::iterator i = m_signatures.find(type);
				RE_ASSERT(i != m_signatures.end(), "System signature not found! Please set an empty sig if your system does not require any component");
				Signature systemSignature = i->second;

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
				{
					// Signature matched.
					CLEARSTRING(out);
					out << "Entity " << entity << " added to " << type.name();
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
						out << "Entity already exists in " << type.name() << ".";
						RE_CORE_INFO(out.str());
					}
				}
				// Entity signature does not match system signature - erase from set/don't add
				else
				{
					CLEARSTRING(out);
					out << "Entity " << entity << " exists in " << type.name();
					RE_CORE_INFO(out.str());
				}
			}
		}

	private:
		std::unordered_map<std::type_index, Signature> m_signatures;
		std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;
	};
}
