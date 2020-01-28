/* Start Header ************************************************************************/
/*!
\file           EditorInspector.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (25%)
				Lim Jia Hao, l.jiahao, 390006518 (25%)
				Chan Wai Kit Terence, c.terence, 440005918 (25%)
				Javier Foo, javier.foo, 440002318 (25%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Coordinator

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "GraphicsSystem.h"
#include "DebugDrawSystem.h"
#include "SceneManager.h"
#include "PhysicsSystem.h"
#include "EventDispatcher.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Types.h"
#include "Timer.h"
#include "AudioSystem.h"
#include "LogicSystem.h"
#include "AudioManager.h"
#include "AnimationSystem.h"
#include "PlayerControllerSystem.h"
#include "MenuControllerSystem.h"

namespace Rogue
{
	class Coordinator
	{
		std::unique_ptr<ComponentManager> m_componentManager;
		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<SystemManager> m_systemManager;
		std::vector<Entity> m_deleteQueue;
		Entity m_assignChild;
		Entity m_assignParent;

	public:
		Coordinator() :
			m_entityManager{ std::make_unique<EntityManager>() },
			m_componentManager{ std::make_unique<ComponentManager>() },
			m_systemManager{ std::make_unique<SystemManager>() },
			m_deleteQueue{},
			m_assignChild{MAX_ENTITIES},
			m_assignParent{MAX_ENTITIES}
		{}

		void Init()
		{
			EventDispatcher::instance().Init();

			// Init the systems and set their signatures.
			m_systemManager->InitSystems();

			SystemInits();

			// Load first scene
			SceneManager::instance().LoadArchetypes("Resources/Archetypes/Archetypes.json");
			SceneManager::instance().LoadLevelFiles("Levels.json");
		}

		void SystemInits()
		{
			GetSystem<AudioSystem>()->TrueInit();
			//GetSystem<AudioSystem>()->InitSounds();

			GetSystem<AnimationSystem>()->ResetTextures();
		}

		void Update()
		{
			// Update the core systems
			m_systemManager->UpdateSystems();

			// If placed before ^, will cause memory leak.
			EventDispatcher::instance().Update();

			ReassignHierarchyParent();
			DeleteEntities();

		}

		void Shutdown()
		{
			m_systemManager->ShutdownSystems();
		}

		Entity CreateEntity()
		{
			return m_entityManager->CreateEntity();
		}

		void DestroyEntity(Entity entity)
		{
			m_entityManager->DestroyEntity(entity);

			m_componentManager->EntityDestroyed(entity);

			m_systemManager->EntityDestroyed(entity);

			//m_systemManager->DeassignTag(entity);
		}

		void DestroyAllEntity()
		{
			m_systemManager->GetSystem<MenuControllerSystem>()->ClearMenuObjs();
			m_systemManager->GetSystem<LogicSystem>()->ClearLogicInterface();

			while (GetActiveObjects().size())
			{
				DestroyEntity(GetActiveObjects().back());
				GetActiveObjects().pop_back();
			}

			SceneManager::instance().ResetObjectIterator();
		}

		Texture loadTexture(const char* texture)
		{
			return TextureManager::instance().loadTexture(texture);
		}

		Shader loadShader(const std::string& shader)
		{
			return ShaderManager::instance().loadShader(shader.data());
		}

		Sound loadSound(const std::string& sound, float volume = 1.0f, bool looping = false)
		{
			return AudioManager::instance().loadSound(sound.data(), volume, looping);
		}

		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();
			m_componentManager->RegisterComponent<T>();

			std::stringstream output;
			output << typeName << " registered!";
			RE_CORE_INFO(output.str());
		}

		template<typename T>
		void AddComponent(Entity entity, const T& component)
		{
			m_componentManager->AddComponent<T>(entity, component);

			auto signature = m_entityManager->GetSignature(entity);
			signature.set(m_componentManager->GetComponentType<T>(), true);
			m_entityManager->SetSignature(entity, signature);

			m_systemManager->EntitySignatureChanged(entity, signature);
			m_systemManager->CreateAssignTagEvent(entity);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			std::cout << "component removed from entity" << std::endl;
			m_componentManager->RemoveComponent<T>(entity);

			auto signature = m_entityManager->GetSignature(entity);
			signature.set(m_componentManager->GetComponentType<T>(), false);
			m_entityManager->SetSignature(entity, signature);

			m_systemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return m_componentManager->GetComponent<T>(entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return m_componentManager->GetComponentType<T>();
		}

		template<typename T>
		T& CreateComponent(Entity owner)
		{
			AddComponent(owner, T());
			return GetComponent<T>(owner);
		}

		template<typename T>
		void CopyComponent(Entity owner, Entity toCopyFrom)
		{
			CreateComponent<T>(owner) = GetComponent<T>(toCopyFrom);
		}

		template<typename T>
		void LoadComponent(Entity owner, std::string_view strToLoad)
		{
			CreateComponent<T>(owner).Deserialize(strToLoad);
		}

		template<typename T>
		void RegisterSystem()
		{
			return m_systemManager->RegisterSystem<T>();
		}

		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			return m_systemManager->GetSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(const Signature& signature)
		{
			m_systemManager->SetSignature<T>(signature);
		}

		template <typename T>
		bool ComponentExists(Entity entity)
		{
			return m_entityManager->GetSignature(entity).test(GetComponentType<T>());
		}
		void InitTimeSystem(const char* system)
		{
			Timer::instance().TimerInit(system);
		}
		void EndTimeSystem(const char* system)
		{
			Timer::instance().TimerEnd(system);
		}

		const std::map<const char*, float>& GetSystemTimes()
		{
			return Timer::instance().GetSystemTimes();
		}

		Entity cloneArchetypes(const char* archetype, bool createHierarchy = true)
		{
			return SceneManager::instance().Clone(archetype, createHierarchy);
		}

		void SaveArchetype(const char* archetype)
		{
			SceneManager::instance().SaveArchetype(archetype);
		}

		Entity clone(Entity existingEntity, bool createHierarchy = true)
		{
			Entity clonedEntity = CreateEntity();
			m_componentManager->clone(existingEntity, clonedEntity);

			Signature newEntitySignature = m_entityManager->GetSignature(existingEntity);
			m_entityManager->SetSignature(clonedEntity, newEntitySignature);
			m_systemManager->EntitySignatureChanged(clonedEntity, newEntitySignature);
			SceneManager::instance().AddToActiveEntities(clonedEntity, createHierarchy);

			return clonedEntity;
			//m_sceneManager->Clone(existingEntity);
		}

		void ResetEvents()
		{
			EventDispatcher::instance().ResetEvents();
		}

		void ClearTimedEntities()
		{
			GetSystem<PlayerControllerSystem>()->ClearTimedEntities();
		}

		void SetHitchhikedEntity(Entity ent)
		{
			GetSystem<PlayerControllerSystem>()->setHitchhikeEntity(ent);
		}

		void ResetHitchhikedEntity()
		{
			GetSystem<PlayerControllerSystem>()->setHitchhikeEntity(-1);
		}

		bool GetLightStatus()
		{
			return GetSystem<PlayerControllerSystem>()->GetLightStatus();
		}

		void ToggleLightStatus()
		{
			GetSystem<PlayerControllerSystem>()->ToggleLightStatus();
		}

		void SetLightStatus(bool status)
		{
			GetSystem<PlayerControllerSystem>()->SetLightStatus(status);
		}
		void AddToDeleteQueue(const Entity& entity)
		{
			m_deleteQueue.push_back(entity);
		}

		void DeleteEntities()
		{
			for (auto& entity : m_deleteQueue)
			{
				DestroyEntity(entity);
				/*for (auto itr = GetActiveObjects().begin(); itr != GetActiveObjects().end(); ++itr)
				{
					if (*itr == entity)
						GetActiveObjects().erase(itr);
				} */
			}

			m_deleteQueue.clear();
		}

		EntityManager& GetEntityManager() const
		{
			return *m_entityManager;
		}

		std::vector <Entity>& GetActiveObjects() const
		{
			return m_entityManager->m_getActiveObjects();
		}

		HierarchyInfo& GetHierarchyInfo(Entity ent)
		{
			return m_entityManager->GetHierarchyInfo(ent);
		}

		std::array<HierarchyInfo, MAX_ENTITIES>& GetHierarchyInfoArray()
		{
			return m_entityManager->GetHierarchyInfoArray();
		}
		
		void SetReassignParentFlags(Entity child, Entity newParent)
		{
			m_assignChild = child;
			m_assignParent = newParent;
		}

		void ReassignHierarchyParent()
		{
			if (m_assignChild == -1 || m_assignChild == MAX_ENTITIES)
				return;

			HierarchyInfo& childInfo = GetHierarchyInfo(m_assignChild);
			HierarchyInfo& newParentInfo = GetHierarchyInfo(m_assignParent);

			if (childInfo.m_parent != -1 && childInfo.m_parent != MAX_ENTITIES)
			{
				HierarchyInfo& oldParentInfo = GetHierarchyInfo(childInfo.m_parent);

				auto end = std::remove(oldParentInfo.m_children.begin(), oldParentInfo.m_children.end(), m_assignChild);
				oldParentInfo.m_children.erase(end, oldParentInfo.m_children.end());
				//oldParentInfo.m_children.erase(std::remove_if(oldParentInfo.m_children.begin(), oldParentInfo.m_children.end(), m_assignChild), oldParentInfo.m_children.end());
			}

			childInfo.m_parent = m_assignParent;
			newParentInfo.m_children.push_back(m_assignChild);

			m_assignChild = -1;
			m_assignParent = -1;
		}
		std::vector<Entity>& getHierarchyVector()
		{

		}
		bool GetGameState() const
		{
			return m_systemManager->GetGameState();
		}

		void SetGameState(bool newGameState)
		{
			m_systemManager->SetGameState(newGameState);
		}

		void ToggleEditorIsRunning()
		{
			m_systemManager->ToggleEditorIsRunning();
		}

		bool GetEditorIsRunning()
		{
			return m_systemManager->GetEditorIsRunning();
		}

		void ToggleGameState()
		{
			m_systemManager->ToggleGameState();
		}

		bool GetPauseState() const
		{
			return m_systemManager->GetPauseState();
		}

		void SetPauseState(bool newPauseState)
		{
			m_systemManager->SetPauseState(newPauseState);
		}

		void TogglePauseState()
		{
			m_systemManager->TogglePauseState();
		}

		bool GameIsActive() const
		{
			return GetGameState() && !GetPauseState();
		}

		void StepOnce() const
		{
			m_systemManager->StepOnce();
		}

		Timer::ChronoTime GetCurrTime() const
		{
			return Timer::instance().GetCurrTime();
		}
		
		void SetStepFrames(size_t frames)
		{
			m_systemManager->SetStepFrames(frames);
		}

		size_t GetStepFrames() const
		{
			return m_systemManager->GetStepFrames();
		}
	};
}