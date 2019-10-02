#include "PhysicsSystem.h"

//-------------------------------------------------------//
//              PRIVATE MEMBER FUNCTIONS				 //
//-------------------------------------------------------//
void PhysicsSystem::applyForces(RigidbodyComponent& rigidbody) // F = ma
{
	std::cout << "AccForce = " << rigidbody.getAccForce() << std::endl;
	rigidbody.offSetAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());
	std::cout << "Acceleration = " << rigidbody.getAcceleration() << std::endl;
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
	//signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	//signature.set(gEngine.m_coordinator.GetComponentType<CircleCollider2DComponent>());
	
	// Set physics system signature.
	gEngine.m_coordinator.SetSystemSignature<PhysicsSystem>(signature);
}

void PhysicsSystem::update()
{
//	RE_CORE_INFO("Running Update");

	Timer TimerSystem;
	// For all entities
	TimerSystem.TimerInit("Physics System");
	for(auto entity : m_entities)
	{
		auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);

		// Skip any static rigidbodies.
		if (rigidbody.getIsStatic())
			continue;

		std::cout << "FUCKFUCKFUCKFUCKFUCKFUCKFUCKFUCKFUCKFUCKFUCKFUCKFUCK" << std::endl;
		auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
	//	auto& boxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);
	//	auto& circleCollider = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(entity);

		// Apply accForce (Forces are added if necessary)
		applyForces(rigidbody);

		// Update positions
		integrateAcceleration(rigidbody, transform);

		// Update collidables
	//	m_colliderManager.updateAABB(boxCollider.m_aabb, transform);
	//	m_colliderManager.updateOBB(boxCollider.m_obb, transform);

		// Conduct spatial partitioning
		
		// Test AABB/OBB Collision
		// gColliderManager.checkCollisions()

		// Collision Response (Contact, forces, etc)
		// Rest, Impulse, Torque

		// Reset accForce
		rigidbody.setAccForce(Vec2());
		
	//	std::cout << "Entity " << entity << "'s pos: " << transform.getPosition() << std::endl;
	}
	TimerSystem.TimerEnd("Physics System");
}

void PhysicsSystem::receive(Event* ev)
{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyPressed:
		{
			std::set<Entity>::iterator it;
			for (it = m_entities.begin(); it != m_entities.end(); ++it)
			{
				auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*it);
				UNREFERENCED_PARAMETER(rigidbody);
				auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(*it);

				KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
				if (EvPressKey->GetKeyCode() == KeyPress::KeyA)
				{
					transform.offSetPosition(Vec2(-1.0f, 0.0f) * gDeltaTime);
					RE_INFO("Move Left!");
				}
				else if (EvPressKey->GetKeyCode() == KeyPress::KeyD)
				{
					transform.offSetPosition(Vec2(1.0f, 0.0f) * gDeltaTime);
					RE_INFO("Move Right!");
				}
				else if (EvPressKey->GetKeyCode() == KeyPress::KeyW)
				{
					transform.offSetPosition(Vec2(0.0f, 1.0f) * gDeltaTime);
					RE_INFO("Move Up!");
				}
				else if (EvPressKey->GetKeyCode() == KeyPress::KeyS)
				{
					transform.offSetPosition(Vec2(0.0f, -1.0f) * gDeltaTime);
					RE_INFO("Move Down!");
				}
				return;
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
