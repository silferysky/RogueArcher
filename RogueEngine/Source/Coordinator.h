#pragma once
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "GraphicsSystem.h"
#include "DebugDrawSystem.h"
#include "ObjectFactory.h"
#include "PhysicsSystem.h"
#include "EventDispatcher.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Coordinator.h"
#include "Types.h"
#include "Timer.h"

class Coordinator
{
	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<ShaderManager> m_shaderManager;
	std::unique_ptr<ObjectFactory> m_objectFactory;

public:
	Coordinator() :
		m_entityManager{ std::make_unique<EntityManager>() },
		m_componentManager{ std::make_unique<ComponentManager>() },
		m_systemManager{ std::make_unique<SystemManager>() },
		m_textureManager{ std::make_unique<TextureManager>() },
		m_shaderManager{ std::make_unique<ShaderManager>() },
		m_objectFactory{ std::make_unique<ObjectFactory>() }
	{}

	void Init()
	{
		// Emplace shaders into the map
		m_shaderManager->Init();

		// Init the systems and set their signatures.
		m_systemManager->InitSystems();

		// Load first scene
		m_objectFactory->LoadLevel("Resources/Level 1.json");
		m_objectFactory->LoadArchetypes("Resources/Archetypes.json");
		//m_objectFactory->Clone("Stuff");
		//m_objectFactory->SaveLevel("Resources/Level 1.json");
	}

	void Update()
	{
		// Update the core systems
		m_systemManager->UpdateSystems();
		EventDispatcher::instance().update(); // Should also be part of systems
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

	GLuint loadTexture(const char* texture)
	{
		return m_textureManager->loadTexture(texture);
	}

	Shader loadShader(std::string shader)
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
	void AddComponent(Entity entity, T component)
	{
		m_componentManager->AddComponent<T>(entity, component);
		
		auto signature = m_entityManager->GetSignature(entity);
		signature.set(m_componentManager->GetComponentType<T>(), true);
		m_entityManager->SetSignature(entity, signature);

		m_systemManager->EntitySignatureChanged(entity, signature);
	}

	void clone(Entity existingEntity)
	{
		Entity clonedEntity = CreateEntity();
		Signature sig = m_entityManager->GetSignature(existingEntity);
		
		for (int i = 0; i < LASTCOMP; i++)
		{
			if (sig.test(i))
			{
				
				auto& existingComponent = GetComponent<RigidbodyComponent>(existingEntity);
				AddComponent(clonedEntity, existingComponent);
			}
		}

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
	void LoadComponent(Entity owner, std::string strToLoad)
	{
		CreateComponent<T>(owner).Deserialize(strToLoad);
	}

	template<typename T>
	void RegisterSystem()
	{
		return m_systemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		m_systemManager->SetSignature<T>(signature);
	}

	EntityManager& GetEntityManager() const
	{
		return *m_entityManager;
	}

	TextureManager& GetTextureManager() const
	{
		return *m_textureManager;
	}

	template <typename T>
	bool CheckIfComponentExists(Entity entity)
	{
		return m_entityManager->GetSignature(entity).test(GetComponentType<T>());
	}
};