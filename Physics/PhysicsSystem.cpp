#include "Physics.h"


// Private member functions





// Public member functions


void PhysicsSystem::positionUpdate(const char* ID)
{
	// Vec2 acceleration = forceComponents[ID].m_force / rigidbodyComponents[ID].m_mass;

	// rigidbodyComponents[ID].m_velocity += acceleration * 0.016f; // dt
	// transformComponents[ID].m_position += rigidbodyComponents[ID].m_velocity * 0.016f; // dt
}

void PhysicsSystem::collisionUpdate(const char* ID)
{
	// For each pair/quad
	// Check bc
	// Check aabb/obb
}


// Public member functions 

void PhysicsSystem::init()
{}

void PhysicsSystem::update()
{
	// Traverse unordered_map
	//{
		// Update forces (Freefall, impulse, torque)

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
