#include "PhysicsSystem.h"


// Private member functions
void PhysicsSystem::applyForces(Rigidbody& rigidbody) // F = ma
{
	rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());
}

void PhysicsSystem::positionUpdate(Rigidbody& rigidbody, Transform& transform, float dt)
{
	rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());

	rigidbody.offSetVelocity(rigidbody.getAcceleration() * dt);
	transform.offSetPosition(rigidbody.getVelocity() * dt);
}

void PhysicsSystem::collisionUpdate(Rigidbody& rigidbody, float dt)
{
	// For each pair/quad
	// Check bc
	// Check aabb/obb
}


// Public member functions 

void PhysicsSystem::init(const Vec2& gravity)
{
	m_gravity = gravity;
}

void PhysicsSystem::update(float dt)
{
	Signature signature;
	signature.set(gCoordinator.GetComponentType<Rigidbody>());
	signature.set(gCoordinator.GetComponentType<Transform>());
//	signature.set(gCoordinator.GetComponentType<BoxCollider2D>());
	signature.set(gCoordinator.GetComponentType<CircleCollider2D>());
	gCoordinator.SetSystemSignature<PhysicsSystem>(signature);

	// Traverse array
	//{
		// Update forces (impulse, torque)

		// Apply forces

		// Update positions
		// positionUpdate("key");

		// Update collidables
	
		// Conduct spatial partitioning

		// Test AABB/OBB Collision
		
		// Do appropriate collision tests
		// collisionUpdate();

		// Collision Response
		// Rest, Impulse, Torque

	//}
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
