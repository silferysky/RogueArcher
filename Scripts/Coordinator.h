#pragma once
#include "EntityManager.h"
#include "ComponentArray.h"
#include "SystemManager.h"
#include "ComponentManager.h"

class Coordinator
{
public:
	void Init()
	{
		// Create pointers to each manager
		REComponentManager = std::make_unique<ComponentManager>();
		REEntityManager = std::make_unique<EntityManager>();
		RESystemManager = std::make_unique<SystemManager>();
	}


	std::uint32_t CreateEntity()
	{
		return REEntityManager->CreateEntity();
	}

	void DestroyEntity(std::uint32_t entity)
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
	void AddComponent(std::uint32_t entity, T component)
	{
		REComponentManager->AddComponent<T>(entity, component);

		auto signature = REEntityManager->GetSignature(entity);
		signature.set(REComponentManager->GetComponentType<T>(), true);
		REEntityManager->SetSignature(entity, signature);

		RESystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(std::uint32_t entity)
	{
		std::cout << "component removed from entity"<<std::endl;
		REComponentManager->RemoveComponent<T>(entity);

		auto signature = REEntityManager->GetSignature(entity);
		signature.set(REComponentManager->GetComponentType<T>(), false);
		REEntityManager->SetSignature(entity, signature);

		RESystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(std::uint32_t entity)
	{
		return REComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	std::uint8_t GetComponentType()
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

private:
	std::unique_ptr<ComponentManager> REComponentManager;
	std::unique_ptr<EntityManager> REEntityManager;
	std::unique_ptr<SystemManager> RESystemManager;
};