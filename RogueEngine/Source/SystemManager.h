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
#include <vector>
#include <utility>
#include "CollisionTagSystem.h"
#include "EventDispatcher.h"
#include "GameEvent.h"

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
			auto i = std::find_if(m_systems.begin(), m_systems.end(),
				[&typeName](const std::pair<std::type_index, std::shared_ptr<System>>& element) { return element.first == typeName; });

			RE_ASSERT(i == m_systems.end(), "Registering system more than once.");

			// Insert the newly created system pointer and typename into the map.
			m_systems.emplace_back(typeName, std::make_shared<T>());
		}

		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			std::type_index SystemName = GetTypeIndex<T>();

			auto i = std::find_if(m_systems.begin(), m_systems.end(),
				[&SystemName](const std::pair<std::type_index, std::shared_ptr<System>>& element) { return element.first == SystemName; });
			
			RE_ASSERT(i != m_systems.end(), "System not found!")

			return std::dynamic_pointer_cast<T>(i->second); // Casts the base shared_ptr to derived
		}

		void InitSystems()
		{
			for (auto system : m_systems)
			{
				system.second->Init();
			}
		}

		void UpdateSystems()
		{
			for (auto system : m_systems)
			{
				// Note: Debug draw system currently doesn't update here.

				// Only run editor if editor is running.
				if (system.second->m_systemID == SystemID::id_EDITOR)
				{
					if (!m_editorIsRunning)
						continue;
				}

				//If either game is not paused or is editor system, run the update
				if ((!m_gameIsPaused && m_gameIsRunning) || 
					(system.second->m_systemID != SystemID::id_PHYSICSSYSTEM))
					system.second->Update();

			}
		}

		void ShutdownSystems()
		{
			for (auto system : m_systems)
			{
				system.second->Shutdown();
			}
		}

		template <typename T>
		void SetSignature(Signature signature)
		{
			std::type_index SystemName = GetTypeIndex<T>();

			// Set the signature for this system
			m_signatures.insert({ SystemName, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			// Erase a destroyed entity from all system lists
			for (auto const& pair : m_systems)
			{
				auto const& system = pair.second;

				system->m_entities.erase(entity);

				std::stringstream str;
				str << "Entity " << entity << " Removed from " << pair.first.name();
				RE_CORE_INFO(str.str());
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			std::stringstream out;

			// Notify each system that an entity's signature changed
			for (auto const& pair : m_systems)
			{
				std::type_index type = pair.first;
				std::shared_ptr<System> system = pair.second;
				std::unordered_map<std::type_index, Signature>::iterator i = m_signatures.find(type);
				RE_ASSERT(i != m_signatures.end(), "System signature not found! Please set an empty sig if your system does not require any component");
				Signature systemSignature = i->second;


				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
				{
					if (system->m_entities.insert(entity).second)
					{
						CLEARSTRING(out);
						out << "Entity " << entity << " added to " << type.name();
						RE_CORE_INFO(out.str());
					}
					else
					{
						CLEARSTRING(out);
						out << "Entity " << entity << " exists in " << type.name();
						RE_CORE_INFO(out.str());
					}
				}
				// Entity signature does not match system signature - erase from set/don't add
				else
				{
					if (system->m_entities.find(entity) != system->m_entities.end()) // If entity exists
					{
						CLEARSTRING(out);
						out << "Entity " << entity << " removed from " << type.name();
						RE_CORE_INFO(out.str());
					}
					else
					{
						CLEARSTRING(out);
						out << "Entity " << entity << " not added to " << type.name();
						RE_CORE_INFO(out.str());
					}
					system->m_entities.erase(entity);
				}
			}
		}

		bool GetGameState() const
		{
			return m_gameIsRunning;
		}

		void SetGameState(bool newGameState)
		{
			m_gameIsRunning = newGameState;
		}

		void ToggleGameState()
		{
			m_gameIsRunning = !m_gameIsRunning;
		}

		bool GetPauseState() const
		{
			return m_gameIsPaused;
		}

		void SetPauseState(bool newPauseState)
		{
			m_gameIsPaused = newPauseState;
		}

		void TogglePauseState()
		{
			m_gameIsPaused = !m_gameIsPaused;
		}

		bool GetEditorIsRunning() const
		{
			return m_editorIsRunning;
		}

		void ToggleEditorIsRunning()
		{
			m_editorIsRunning = m_editorIsRunning ?
				false :
				true;
		}

		void CreateAssignTagEvent(Entity entity)
		{
			EntChangeStateEvent* changeStateEvent = new EntChangeStateEvent(entity, 0);
			changeStateEvent->SetSystemReceivers((int)SystemID::id_COLLISIONTAGSYSTEM);
			EventDispatcher::instance().AddEvent(changeStateEvent);
		}

		void DeassignTag(Entity entity)
		{
			GetSystem<CollisionTagSystem>()->DeassignTag(entity);
		}

	private:
		std::unordered_map<std::type_index, Signature> m_signatures;
		std::vector<std::pair<std::type_index, std::shared_ptr<System>>> m_systems;
		bool m_gameIsRunning = false;
		bool m_gameIsPaused = false;
		bool m_editorIsRunning = true;
	};
}