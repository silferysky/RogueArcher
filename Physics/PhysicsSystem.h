#pragma once
#include "CollisionManager.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "REMath.h"

class PhysicsSystem
{
	CollisionManager gColliderManager;
	Vec2 m_gravity;

	void positionUpdate(Rigidbody& rigidbody, Transform& transform, float dt);
	void collisionUpdate(Rigidbody& rigidbody, float dt);
	void applyForces(Rigidbody& rigidbody);

public:
	PhysicsSystem() = default;
	~PhysicsSystem() = default;

	void init(const Vec2& gravity);
	void update(float dt);
};