/* Start Header ************************************************************************/
/*!
\file           SystemManager.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for SystemManager

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
#include "Logger.h"
#include "Event.h"
#include "SystemList.h"
#include "BaseSystem.h"
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
			for (auto& system : m_systems)
			{
				system.second->Init();
			}
		}

		void UpdateSystems();

		void ShutdownSystems()
		{
			for (auto& system : m_systems)
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

#if ENABLE_CORE_LOGGING
				std::stringstream str;
				str << "Entity " << entity << " Removed from " << pair.first.name();
				RE_CORE_INFO(str.str());
#endif
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
#if ENABLE_CORE_LOGGING
			std::stringstream out;
#endif

			// Notify each system that an entity's signature changed
			for (auto const& pair : m_systems)
			{
				std::type_index type = pair.first;
				std::shared_ptr<System> system = pair.second;
				std::unordered_map<std::type_index, Signature>::iterator i = m_signatures.find(type);

				RE_ASSERT(i != m_signatures.end(), "System signature not found! Please set an empty signature if your system does not require any component");
				
				Signature systemSignature = i->second;

				// Entity signature matches system signature - insert into set

				if ((entitySignature & systemSignature) == systemSignature)
				{
					if (system->m_entities.insert(entity).second)
					{
#if ENABLE_CORE_LOGGING
						CLEARSTRING(out);
						out << "Entity " << entity << " added to " << type.name();
						RE_CORE_INFO(out.str());
					}
					else
					{
						CLEARSTRING(out);
						out << "Entity " << entity << " exists in " << type.name();
						RE_CORE_INFO(out.str());
#endif
					}
				}
				// Entity signature does not match system signature - erase from set/don't add
				else
				{
#if ENABLE_CORE_LOGGING
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
#endif
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
			m_gameModeChanged = true;

			m_editorIsRunning = m_editorIsRunning ?
				false :
				true;
		}

		void StepOnce()
		{
			m_stepOnce = true;
		
			if(m_stepFrames)
				m_stepCounter = static_cast<int>(m_stepFrames);
		}

		void CreateAssignTagEvent(Entity entity)
		{
			EntChangeStateEvent changeStateEvent(entity, 0);
			changeStateEvent.SetSystemReceivers((int)SystemID::id_COLLISIONTAGSYSTEM);
			EventDispatcher::instance().AddEvent(changeStateEvent);
		}

		void SetStepFrames(size_t count)
		{
			m_stepFrames = count;
		}

		size_t GetStepFrames() const
		{
			return m_stepFrames;
		}

		void SetTransitionLevel(std::string_view levelName, float transitTime);	//Setting Transition Level, but not transitioning yet
		void SetTransition(bool transition = true);								//Actually telling system to transition
		bool TransitFinish() const;

	private:
		std::unordered_map<std::type_index, Signature> m_signatures;
		std::vector<std::pair<std::type_index, std::shared_ptr<System>>> m_systems;
		bool m_gameIsRunning = false;
		bool m_gameIsPaused = false;
		bool m_stepOnce = false;
		bool m_gameModeChanged = false;
		bool m_repeatLoad = false;

#if INIT_EDITOR
		bool m_editorIsRunning = true;
#else
		bool m_editorIsRunning = false;
#endif
#if INIT_CURSOR
		bool m_showCursor = true;
#else
		bool m_showCursor = false;
#endif

		bool m_transitionLevel = false;
		float m_transitionTime = 0.0f;
		std::string m_transitionString = "";

		size_t m_stepFrames = 1;
		int m_stepCounter = 0;

		void FixedUpdate();
		void Update();

		void UpdateSystem(SystemID id) { m_systems[static_cast<int>(id)].second->Update(); }
		void FixedUpdateSystem(SystemID id, const char* systemName);
	};
}