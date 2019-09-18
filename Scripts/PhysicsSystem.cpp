#include "Main.h"


// Private member functions





// Public member functions

void PhysicsSystem::computeForce(int ID)
{
	m_rigidbodies[ID].m_force = m_rigidbodies[ID].m_shape.m_mass * m_world.m_gravity;
}

void PhysicsSystem::positionUpdate(int ID)
{
	Vec2 acceleration = { m_rigidbodies[ID].m_force / m_rigidbodies[ID].m_shape.m_mass };

	m_rigidbodies[ID].m_vel += acceleration * 0.016; // dt
	m_rigidbodies[ID].m_pos += m_rigidbodies[ID].m_vel * 0.016; // dt
}

void PhysicsSystem::collisionUpdate(int ID)
{
	int lastID = PHYS_MAX_COLLIDERS - 1;

	// TODO: For sorted array of colliders, test collision in pairs
	
	if (ID == lastID)
		return;

	m_colliders[ID].m_orientedBox.CollisionTest(m_colliders[ID + 1].m_orientedBox);
}


// Public member functions 

void PhysicsSystem::init(const Vec2& gravity)
{
	// Set gravity
	m_world.setGravity(gravity);

	// Prefab should init the shapes of each rigidbody
	for (auto rigidbody : m_rigidbodies)
	{
		// Set all rigidbodies to be 10 width, 10 height, 10 mass
		//rigidbody.m_shape.initShape(10, 10, 10);
	}

	// Prefab should init the vertices of each collider
	for (auto collider : m_colliders)
	{
		// Set all OBB as ship
		//collider.m_orientedBox.initCollider(OBB_SHIP);
	}
}

void PhysicsSystem::update()
{
	for (int i = 0; i < PHYS_MAX_RIGIDBODIES; ++i)
	{
		// Update forces
		computeForce(i);

		// Update positions
		positionUpdate(i);

		// Update AABB/OBB and spatial partition


		// Test AABB/OBB Collision
		collisionUpdate(i);

		// Collision Response
	}
}

void World::setGravity(const Vec2 &grav)
{
	m_gravity = grav;
}
