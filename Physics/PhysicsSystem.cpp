#include "PhysicsSystem.h"

//-------------------------------------------------------//
//              PRIVATE MEMBER FUNCTIONS					 //
//-------------------------------------------------------//
void PhysicsSystem::applyForces(Rigidbody& rigidbody) // F = ma
{
	rigidbody.offSetAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());
}

PhysicsSystem::PhysicsSystem()
	: m_colliderManager{}, m_gravity{0.0f, -9.81f}
{}

void PhysicsSystem::integrateAcceleration(Rigidbody& rigidbody, Transform& transform)
{
	Vec2 vel = rigidbody.getAcceleration() * gDeltaTime;
	vel *= static_cast<float>(std::pow(rigidbody.getDamping(), gDeltaTime));
	rigidbody.offSetVelocity(vel);
	transform.offSetPosition(rigidbody.getVelocity() * gDeltaTime);

}


//-------------------------------------------------------//
//              PUBLIC MEMBER FUNCTIONS					 //
//-------------------------------------------------------//
void PhysicsSystem::init()
{
	// Add components to signature.
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<Rigidbody>());
	signature.set(gEngine.m_coordinator.GetComponentType<Transform>());
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2D>());
//	signature.set(gEngine.m_coordinator.GetComponentType<CircleCollider2D>());
	
	// Set physics system signature.
	gEngine.m_coordinator.SetSystemSignature<PhysicsSystem>(signature);

	for (auto entity : m_entities)
	{
		auto& rigidbodyCmp = gEngine.m_coordinator.GetComponent<Rigidbody>(entity);
		rigidbodyCmp.setAcceleration(m_gravity);
	}
}

void PhysicsSystem::update()
{
//	RE_CORE_INFO("Running Update");

	// For all entities
	for(auto entity : m_entities)
	{
		auto& rigidbody = gEngine.m_coordinator.GetComponent<Rigidbody>(entity);
		auto& transform = gEngine.m_coordinator.GetComponent<Transform>(entity);
		auto& boxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2D>(entity);
		auto& circleCollider = gEngine.m_coordinator.GetComponent<CircleCollider2D>(entity);

		// Reset accForce
		rigidbody.setAccForce(Vec2());

		// Apply accForce (Forces are added if necessary)
		applyForces(rigidbody);

		// Update positions
		integrateAcceleration(rigidbody, transform);

		// Update collidables
		m_colliderManager.updateAABB(boxCollider.m_aabb, transform);
		m_colliderManager.updateOBB(boxCollider.m_obb, transform);

		// Conduct spatial partitioning
		
		// Test AABB/OBB Collision
		// gColliderManager.checkCollisions()

		// Collision Response (Contact, forces, etc)
		// Rest, Impulse, Torque
		
		//	std::cout << "Entity " << entity << "'s pos: " << transform.getPosition() << std::endl;
	}
}

void PhysicsSystem::receive(Event* ev)
{
	RE_INFO(ev->ToString());
	RE_CORE_INFO("TEST SYSTEM RECEIVED EVENT");
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
