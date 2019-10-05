#pragma once
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "FileManager.h"
#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include "EventDispatcher.h"

// Forward declaration
class PhysicsSystem;
class GraphicsSystem;
class SpriteComponent;
class EventDispatcher;

class Coordinator
{
	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
	std::unique_ptr<FileManager> m_fileManager;
	std::unique_ptr<TextureManager> m_textureManager;

	bool m_togglePerformanceChecker = true;
	bool m_fpscheck = true;
public:
	Coordinator()
		:	m_entityManager{ std::make_unique<EntityManager>() },
			m_componentManager{ std::make_unique<ComponentManager>() },
			m_systemManager{ std::make_unique<SystemManager>() },
			m_fileManager{ std::make_unique<FileManager>() },
			m_textureManager{ std::make_unique<TextureManager>() },
			m_togglePerformanceChecker{ false },
			m_fpscheck{ false }
	{}

	void Init()
	{
		// Init the systems and set their signatures.
		m_systemManager->InitSystems();
	}

	void Update()
	{
		// Update the core systems
		m_systemManager->UpdateSystems();
		EventDispatcher::instance().update();
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

	std::shared_ptr<std::string> getVertexShader() const
	{
		return m_fileManager->getVertexShader();
	}

	std::shared_ptr<std::string> getFragmentShader() const
	{
		return m_fileManager->getFragmentShader();
	}
	
	template <typename T>
	bool CheckIfComponentExists(Entity entity)
	{
		return m_entityManager->GetSignature(entity).test(GetComponentType<T>());
	}

	void togglePerformanceChecker()
	{
		m_togglePerformanceChecker = !m_togglePerformanceChecker;
	}

	bool performanceChecker()
	{
		return m_togglePerformanceChecker;
	}

	void toggleFPSChecker()
	{
		m_fpscheck = !m_fpscheck;
	}

	bool FPSChecker()
	{
		return m_fpscheck;
	}

};