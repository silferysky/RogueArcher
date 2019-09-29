#pragma once
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
//#include "ObjectFactory.h"
#include "GraphicsSystem.h"
#include "../Physics/PhysicsSystem.h"
#include "Logger.h"

// Forward declaration
class PhysicsSystem;
class GraphicsSystem;
//class ObjectFactory;

class Coordinator
{
public:
	Coordinator()
		: m_entityManager{ std::make_unique<EntityManager>() },
		  m_componentManager{ std::make_unique<ComponentManager>() },
		  m_systemManager{ std::make_unique<SystemManager>() },
		  m_activeEntities{MAX_ENTITIES}
	{}

	void Init()
	{
		// Register all systems.
		auto PhysSystem = RegisterSystem<PhysicsSystem>();
		auto graphics = RegisterSystem<GraphicsSystem>();

		// Init systems and system signatures will be set in their respective inits.
		initSystems();

		// Register all components
		RegisterComponent<SpriteComponent>();
		RegisterComponent<Rigidbody>();
		RegisterComponent<Transform>();
		RegisterComponent<CircleCollider2D>();
		RegisterComponent<BoxCollider2D>();

		// Create entities (Temporary component creation)
		for (auto entity : m_activeEntities)
		{
			entity = CreateEntity();
		 	AddComponent<SpriteComponent>(entity, SpriteComponent{});
			AddComponent<Rigidbody>(entity, Rigidbody{});
			AddComponent<Transform>(entity, Transform{});
			AddComponent<BoxCollider2D>(entity, BoxCollider2D{});
		}

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
		std::cout << "component removed from entity"<<std::endl;
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

	ComponentType GetComponentType(const char* typeName)
	{
		return m_componentManager->GetComponentType(typeName);
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

	size_t Size(const char* name) const
	{
		if (name == "Entity")
			return m_entityManager->Size();
		else if (name == "Component")
			return m_componentManager->Size();
		else
			return 0;
	}

	EntityManager& GetEntityManager() const
	{
		return *m_entityManager;
	}

private:
	void initSystems()
	{
		m_systemManager->initSystems();
	}

//	void generateEntities()
//	{
		//RE_CORE_INFO("TEST OBJECT FACTORY");
		//std::stringstream debugStr;
		//size_t objInLevel = Size("Entity");
		//
		//debugStr << "Number of entities at start: " << objInLevel;
		//
		//RE_CORE_INFO(debugStr.str());
		//objFac.LoadLevel("Resources/Level 1.json");

		//This section is commented out to prevent issues when spawning entities
		/*Entity e1 = gEngine.m_coordinator.CreateEntity();
		SpriteComponent s;
		Rigidbody r;
		Transform t;
		CircleCollider2D cc;

		s.set = 0;
		s.VAO = 0;
		s.VBO = 0;
		s.EBO = 0;
		t.setPosition(Vec2(0.0f, 0.0f));
		t.setScale(Vec2(0.0f, 0.0f));
		t.setRotation(0.0f);
		cc.setRadius(0.0f);
		r.setAcceleration(Vec2(0.0f, 0.0f));
		r.setMass(1.0f);
		r.setVelocity(Vec2(0.0f, 0.0f));
		r.setVolume(0.0f);

		gEngine.m_coordinator.AddComponent(e1, s);
		gEngine.m_coordinator.AddComponent(e1, r);
		gEngine.m_coordinator.AddComponent(e1, t);
		gEngine.m_coordinator.AddComponent(e1, cc);
		RE_INFO(gEngine.m_coordinator.GetEntityManager().GetSignature(e1).to_ulong());

		This is used as a "macro" to reset values in json file
		gEngine.m_coordinator.AddComponent(e2, s);
		gEngine.m_coordinator.AddComponent(e2, r);
		gEngine.m_coordinator.AddComponent(e2, t);
		gEngine.m_coordinator.AddComponent(e2, cc);
		gEngine.m_coordinator.AddComponent(e3, s);
		gEngine.m_coordinator.AddComponent(e3, r);
		gEngine.m_coordinator.AddComponent(e3, t);
		gEngine.m_coordinator.AddComponent(e3, cc);
		gEngine.m_coordinator.AddComponent(e4, s);
		gEngine.m_coordinator.AddComponent(e4, r);
		gEngine.m_coordinator.AddComponent(e4, t);
		gEngine.m_coordinator.AddComponent(e4, cc);
		gEngine.m_coordinator.AddComponent(e5, s);
		gEngine.m_coordinator.AddComponent(e5, r);
		gEngine.m_coordinator.AddComponent(e5, t);
		gEngine.m_coordinator.AddComponent(e5, cc);*/
	
		//objFac.SaveLevel("Resources/Level 1.json");
		//objInLevel = Size("Entity");
		//debugStr << "Number of entities at end: " << objInLevel;
		//RE_INFO(debugStr.str());
	//}

	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;

	//ObjectFactory objFac;
	std::vector<Entity> m_activeEntities;
};