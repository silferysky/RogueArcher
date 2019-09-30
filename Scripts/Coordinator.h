#pragma once
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "FileManager.h"
#include "GraphicsSystem.h"
#include "../Physics/PhysicsSystem.h"

// Forward declaration
class PhysicsSystem;
class GraphicsSystem;

class Coordinator
{
public:
	Coordinator()
		: m_entityManager{ std::make_unique<EntityManager>() },
		  m_componentManager{ std::make_unique<ComponentManager>() },
		  m_systemManager{ std::make_unique<SystemManager>() },
		  m_fileManager{ std::make_unique<FileManager>() },
		  m_textureManager{ std::make_unique<TextureManager>() },
		  m_activeEntities{MAX_ENTITIES}
	{}

	void Init()
	{
		RE_CORE_INFO("===============COORDINATOR INIT===============");

		// Register all systems.
		RE_CORE_INFO("-----------START REGISTERING SYSTEMS----------");

		auto PhysSystem = RegisterSystem<PhysicsSystem>();
		auto graphics = RegisterSystem<GraphicsSystem>();

		RE_CORE_INFO("-----------END REGISTERING SYSTEMS------------\n\n");

		// Register all components
		RE_CORE_INFO("---------START REGISTERING COMPONENTS---------");

		RegisterComponent<SpriteComponent>();
		RegisterComponent<RigidbodyComponent>();
		RegisterComponent<TransformComponent>();
		RegisterComponent<CircleCollider2DComponent>();
		RegisterComponent<BoxCollider2DComponent>();

		RE_CORE_INFO("----------END REGISTERING COMPONENTS----------\n\n");

		// Init systems and system signatures will be set in their respective inits.
		RE_CORE_INFO("----------START INITIALIZING SYSTEMS----------");

		initSystems();

		RE_CORE_INFO("-----------END INITIALIZING SYSTEMS-----------\n\n");

	}

	void update()
	{
		//...
		m_systemManager->updateSystems();
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
	std::shared_ptr<T> RegisterSystem()
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

private:
	void initSystems()
	{
		m_systemManager->initSystems();
	}

	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
	std::unique_ptr<FileManager> m_fileManager;
	std::unique_ptr<TextureManager> m_textureManager;
	std::vector<Entity> m_activeEntities;
};