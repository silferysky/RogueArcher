#pragma once
#include "SystemManager.h"
#include "ComponentManager.h"

#include "SpriteComponent.h"
#include "../Physics/Rigidbody.h"
#include "../Physics/Transform.h"
#include "../Physics/CircleCollider2D.h"
#include "../BoxCollider2D.h"
#include "../Physics/PhysicsSystem.h"


class Coordinator
{
public:

	void Init()
	{
		// Create managers
		REComponentManager = std::make_unique<ComponentManager>();
		REEntityManager = std::make_unique<EntityManager>();
		RESystemManager = std::make_unique<SystemManager>();

		// Register all components
		RegisterComponent<SpriteComponent>();
		RegisterComponent<Rigidbody>();
		RegisterComponent<Transform>();
		RegisterComponent<CircleCollider2D>();
		RegisterComponent<BoxCollider2D>();

		m_activeEntities.reserve(MAX_ENTITIES - 1);

		// Create entities (Temporary component creation)
		for (auto& entity : m_activeEntities)
		{
			entity = CreateEntity();
			AddComponent<SpriteComponent>(entity, SpriteComponent{ 0, 0, 0, 0 });
			AddComponent<Rigidbody>(entity, Rigidbody{});
			AddComponent<Transform>(entity, Transform{});
			AddComponent<BoxCollider2D>(entity, BoxCollider2D{});
		}
	}

	void initSystems()
	{
		RESystemManager->initSystems();
	}

	void update()
	{
		//...
		RESystemManager->updateSystems();
	}

	Entity CreateEntity()
	{
		return REEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		REEntityManager->DestroyEntity(entity);

		REComponentManager->EntityDestroyed(entity);

		RESystemManager->EntityDestroyed(entity);
	}

	template<typename T>
	void RegisterComponent()
	{
		REComponentManager->RegisterComponent<T>();
		std::cout << "Components Created" << std::endl;
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		REComponentManager->AddComponent<T>(entity, component);

		auto signature = REEntityManager->GetSignature(entity);
		signature.set(REComponentManager->GetComponentType<T>(), true);
		REEntityManager->SetSignature(entity, signature);

		RESystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		std::cout << "component removed from entity"<<std::endl;
		REComponentManager->RemoveComponent<T>(entity);

		auto signature = REEntityManager->GetSignature(entity);
		signature.set(REComponentManager->GetComponentType<T>(), false);
		REEntityManager->SetSignature(entity, signature);

		RESystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return REComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return REComponentManager->GetComponentType<T>();
	}

	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return RESystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		RESystemManager->SetSignature<T>(signature);
	}

	size_t Size(const char* name) const
	{
		if (name == "Entity")
			return REEntityManager->Size();
		else if (name == "Component")
			return REComponentManager->Size();
		else
			return 0;
	}

	EntityManager& GetEntityManager() const
	{
		return *REEntityManager;
	}

private:
	std::unique_ptr<ComponentManager> REComponentManager;
	std::unique_ptr<EntityManager> REEntityManager;
	std::unique_ptr<SystemManager> RESystemManager;

	std::vector<Entity> m_activeEntities;
};