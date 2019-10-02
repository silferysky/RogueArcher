#include "PhysicsSystem.h"

//-------------------------------------------------------//
//              PRIVATE MEMBER FUNCTIONS				 //
//-------------------------------------------------------//
void PhysicsSystem::applyForces(RigidbodyComponent& rigidbody) // F = ma
{
	rigidbody.offSetAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());
}

PhysicsSystem::PhysicsSystem(Vec2 gravity)
	: m_colliderManager{}, m_gravity{gravity}
{}

void PhysicsSystem::integrateAcceleration(RigidbodyComponent& rigidbody, TransformComponent& transform)
{
	transform.offSetPosition(rigidbody.getVelocity() * gDeltaTime);

	Vec2 vel = rigidbody.getAcceleration() * gDeltaTime;
//	std::cout << "Vel = " << vel << std::endl;

	vel *= static_cast<float>(std::pow(rigidbody.getDamping(), gDeltaTime));
	rigidbody.offSetVelocity(vel);

}


//-------------------------------------------------------//
//              PUBLIC MEMBER FUNCTIONS					 //
//-------------------------------------------------------//
void PhysicsSystem::init()
{
	LISTENER_HANDLER hand = std::bind(&PhysicsSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_PHYSICSSYSTEM, hand);
	// Add components to signature.
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<RigidbodyComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	//signature.set(gEngine.m_coordinator.GetComponentType<CircleCollider2DComponent>());
	
	// Set physics system signature.
	gEngine.m_coordinator.SetSystemSignature<PhysicsSystem>(signature);
}

void PhysicsSystem::update()
{
//	RE_CORE_INFO("Running Update");

	Timer TimerSystem;
	TimerSystem.TimerInit("Physics System");

	// For all entities
	std::set<Entity>::iterator iEntity;
	for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
	{
		auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

		// Skip any static rigidbodies.
		if (rigidbody.getIsStatic())
			continue;

//		std::cout << "Entity " << *iEntity << std::endl;
		auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
		auto& currBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
	//	auto& circleCollider = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

		// Apply accForce (Forces are added if necessary)
		applyForces(rigidbody);

		// Update positions
		integrateAcceleration(rigidbody, transform);

		// Update collidables
		m_colliderManager.updateAABB(currBoxCollider.m_aabb, transform);
//		m_colliderManager.updateOBB(boxCollider.m_obb, transform);

		// Conduct spatial partitioning
		
		// Test AABB/OBB Collision
		std::set<Entity>::iterator iNextEntity = iEntity;
		for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
		{
			auto& nextBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
			if (m_colliderManager.staticAABBvsAABB(currBoxCollider.m_aabb, nextBoxCollider.m_aabb))
				std::cout << "Entity " << *iEntity << " AABB collides with Entity " << *iNextEntity << " AABB" << std::endl;
		}
		// Collision Response (Contact, forces, etc)
		// Rest, Impulse, Torque

		// Reset accForce
		rigidbody.setAccForce(Vec2());
		
	//	std::cout << "Entity " << iEntity << "'s pos: " << transform.getPosition() << std::endl;
	}
	TimerSystem.TimerEnd("Physics System");
}

void PhysicsSystem::receive(Event* ev)
{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyPressed:
		{
		//	auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
		//	UNREFERENCED_PARAMETER(rigidbody);

			KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
				
			if (EvPressKey->GetKeyCode() == KeyPress::KeyA)
			{
				for (auto entity : m_entities)
				{
					if (entity == 1) // Entity A
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(-1.0f, 0.0f) * gDeltaTime);
						RE_INFO("Move A Left!");
					}
				}
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyD)
			{
				for (auto entity : m_entities)
				{
					if (entity == 1) // Entity A
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(1.0f, 0.0f) * gDeltaTime);
						RE_INFO("Move A Right!");
					}
				}
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyW)
			{
				for (auto entity : m_entities)
				{
					if (entity == 1) // Entity A
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(0.0f, 1.0f) * gDeltaTime);
						RE_INFO("Move A Up!");
					}
				}
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyS)
			{
				for (auto entity : m_entities)
				{
					if (entity == 1) // Entity A
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(0.0f, -1.0f) * gDeltaTime);
						RE_INFO("Move A Down!");
					}
				}
			}
			if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowLeft)
			{
				for (auto entity : m_entities)
				{
					if (entity == 2) // Entity B
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(-1.0f, 0.0f) * gDeltaTime);
						RE_INFO("Move B Left!");
					}
				}
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowRight)
			{
				for (auto entity : m_entities)
				{
					if (entity == 2) // Entity B
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(1.0f, 0.0f) * gDeltaTime);
						RE_INFO("Move B Right!");
					}
				}
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowUp)
			{
				for (auto entity : m_entities)
				{
					if (entity == 2) // Entity B
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(0.0f, 1.0f) * gDeltaTime);
						RE_INFO("Move B Up!");
					}
				}
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowDown)
			{
				for (auto entity : m_entities)
				{
					if (entity == 2) // Entity B
					{
						auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
						transform.offSetPosition(Vec2(0.0f, -1.0f) * gDeltaTime);
						RE_INFO("Move B Down!");
					}
				}
			}
		}
		default:
			return;
	}
}

// Setters
void PhysicsSystem::setGravity(const Vec2& gravity)
{
	m_gravity = gravity;
}

const Vec2& PhysicsSystem::getGravity() const
{
	return m_gravity;
}
