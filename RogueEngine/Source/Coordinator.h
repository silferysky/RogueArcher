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
#include "Coordinator.h"
#include "Types.h"
#include "Timer.h"
#include "AudioSystem.h"

namespace Rogue
{
	class Coordinator
	{
		std::unique_ptr<ComponentManager> m_componentManager;
		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<SystemManager> m_systemManager;
		std::unique_ptr<TextureManager> m_textureManager;
		std::unique_ptr<ShaderManager> m_shaderManager;
		std::unique_ptr<SceneManager> m_sceneManager;
		std::unique_ptr<EventDispatcher> m_eventDispatcher;
		std::unique_ptr<Timer> m_timer;

	public:
		Coordinator() :
			m_entityManager{ std::make_unique<EntityManager>() },
			m_componentManager{ std::make_unique<ComponentManager>() },
			m_systemManager{ std::make_unique<SystemManager>() },
			m_textureManager{ std::make_unique<TextureManager>() },
			m_shaderManager{ std::make_unique<ShaderManager>() },
			m_sceneManager{ std::make_unique<SceneManager>() },
			m_eventDispatcher{ std::make_unique<EventDispatcher>() },
			m_timer{ std::make_unique<Timer>() }
		{}

		void Init()
		{
			// Init the systems and set their signatures.
			m_systemManager->InitSystems();

			// Load first scene
			m_sceneManager->LoadLevelFiles("Levels.json");
			m_sceneManager->LoadLevel("Level 1.json");
			m_sceneManager->LoadArchetypes("Resources/Archetypes.json");

			GetSystem<AudioSystem>()->InitSounds();
		}

		void Update()
		{
			// Update the core systems
			m_systemManager->UpdateSystems();

			// If placed before ^, will cause memory leak.
			EventDispatcher::instance().Update();
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
		}

		void DestroyAllEntity()
		{
			while (GetActiveObjects().size())
			{
				DestroyEntity(GetActiveObjects().back().m_Entity);
				GetActiveObjects().pop_back();
			}
			m_sceneManager->ResetObjectIterator();
		}

		Texture loadTexture(const char* texture)
		{
			return m_textureManager->loadTexture(texture);
		}

		Shader loadShader(const std::string& shader)
		{
			return m_shaderManager->loadShader(shader);
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
		void LoadComponent(Entity owner, const std::string& strToLoad)
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
			m_timer->TimerInit(system);
		}
		void EndTimeSystem(const char* system)
		{
			m_timer->TimerEnd(system);
		}

		const std::map<const char*, float>& GetSystemTimes()
		{
			return m_timer->GetSystemTimes();
		}

		void cloneArchetypes(const char* archetype)
		{
			m_sceneManager->Clone(archetype);
		}

		void clone(Entity existingEntity)
		{
			Entity clonedEntity = CreateEntity();
			m_componentManager->clone(existingEntity, clonedEntity);

			Signature newEntitySignature = m_entityManager->GetSignature(existingEntity);
			m_entityManager->SetSignature(clonedEntity, newEntitySignature);
			m_systemManager->EntitySignatureChanged(clonedEntity, newEntitySignature);
			m_sceneManager->AddToActiveEntities(clonedEntity);

			//m_sceneManager->Clone(existingEntity);
		}

		EntityManager& GetEntityManager() const
		{
			return *m_entityManager;
		}

		TextureManager& GetTextureManager() const
		{
			return *m_textureManager;
		}

		ShaderManager& GetShaderManager() const
		{
			return *m_shaderManager;
		}

		SceneManager& GetSceneManager() const
		{
			return *m_sceneManager;
		}

		std::vector <HierarchyInfo>& GetActiveObjects()
		{
			return m_entityManager->m_getActiveObjects();
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

		Timer::ChronoTime GetCurrTime() const
		{
			return m_timer->GetCurrTime();
		}
	};
}