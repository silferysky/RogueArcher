#pragma once
#define PHYS_MAX_RIGIDBODIES 256
#define PHYS_MAX_COLLIDERS 256

class World
{
	Vec2 m_gravity;

public:
	World(Vec2 gravity = {0.0f, -9.81f})
		: m_gravity{gravity}
	{}

	void setGravity(const Vec2 &grav);

	friend class PhysicsSystem;
};

class PhysicsSystem
{
	World m_world;
	std::array<RigidbodyComponent, PHYS_MAX_RIGIDBODIES> m_rigidbodies;
	std::array<ColliderComponent, PHYS_MAX_COLLIDERS> m_colliders;

	void computeForce(int ID);
	void positionUpdate(int ID);
	void collisionUpdate(int ID);

public:
	PhysicsSystem()
	{}

	~PhysicsSystem()
	{}

	void init(const Vec2& gravity);
	void update();
};