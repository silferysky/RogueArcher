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

namespace Rogue
{
	class Coordinator
	{
		std::unique_ptr<ComponentManager> m_componentManager;
		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<SystemManager> m_systemManager;
		std::vector<Entity> m_deleteQueue;
	public:
		Coordinator() :
			m_entityManager{ std::make_unique<EntityManager>() },
			m_componentManager{ std::make_unique<ComponentManager>() },
			m_systemManager{ std::make_unique<SystemManager>() },
			m_deleteQueue{}
		{}

		void Init()
		{
			// Init the systems and set their signatures.
			m_systemManager->InitSystems();

			// Load first scene
			SceneManager::instance().LoadLevelFiles("Levels.json");
			SceneManager::instance().LoadArchetypes("Resources/Archetypes/Archetypes.json");

			SystemInits();
		}

		void SystemInits()
		{
			GetSystem<AudioSystem>()->TrueInit();
			GetSystem<AudioSystem>()->InitSounds();

			GetSystem<AnimationSystem>()->ResetTextures();
		}

		void Update()
		{
			// Update the core systems
			m_systemManager->UpdateSystems();

			// If placed before ^, will cause memory leak.
			EventDispatcher::instance().Update();

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
			while (m_entityManager->m_getActiveObjects().size())
			{
				DestroyEntity(GetActiveObjects().back());
				GetActiveObjects().pop_back();
			}

			m_systemManager->GetSystem<LogicSystem>()->ClearLogicInterface();
			SceneManager::instance().ResetObjectIterator();
		}

		Texture loadTexture(const char* texture)
		{
			return TextureManager::instance().loadTexture(texture);
		}

		Shader loadShader(std::string_view shader)
		{
			return ShaderManager::instance().loadShader(shader.data());
		}

		Sound loadSound(std::string_view sound)
		{
			return AudioManager::instance().loadSound(sound.data());
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

		void cloneArchetypes(const char* archetype)
		{
			SceneManager::instance().Clone(archetype);
		}

		void SaveArchetype(const char* archetype)
		{
			SceneManager::instance().SaveArchetype(archetype);
		}

		void clone(Entity existingEntity)
		{
			Entity clonedEntity = CreateEntity();
			m_componentManager->clone(existingEntity, clonedEntity);

			Signature newEntitySignature = m_entityManager->GetSignature(existingEntity);
			m_entityManager->SetSignature(clonedEntity, newEntitySignature);
			m_systemManager->EntitySignatureChanged(clonedEntity, newEntitySignature);
			SceneManager::instance().AddToActiveEntities(clonedEntity);

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

		void AddToDeleteQueue(const Entity& entity)
		{
			m_deleteQueue.push_back(entity);
		}

		void DeleteEntities()
		{
			for (auto& entity : m_deleteQueue)
			{
				DestroyEntity(entity);
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