#pragma once
#include "../Scripts/Main.h"
#include "CollisionManager.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "../Scripts/REMath.h"
#include "../Scripts/BaseSystem.h"
#include "../Scripts/EventListener.h"

class PhysicsSystem : public BaseSystem, public EventListener
{
	CollisionManager gColliderManager;
	Vec2 m_gravity;

	void positionUpdate(Rigidbody& rigidbody, Transform& transform, float dt);
	void collisionUpdate(Rigidbody& rigidbody, float dt);
	void applyForces(Rigidbody& rigidbody);

public:
	PhysicsSystem() = default;
	~PhysicsSystem() = default;

	// Init system with gravity (default 0,0)
	void init(const Vec2& gravity = Vec2());
	void update(float dt);

	void setGravity(const Vec2& gravity);
	const Vec2& getGravity() const;
};