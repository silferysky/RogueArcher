#pragma once
#include "../Scripts/Main.h"
#include "CollisionManager.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "../Scripts/REMath.h"
#include "../Scripts/EventListener.h"
#include "../BoxCollider2D.h"
#include <iostream>

class PhysicsSystem : public System
{
	CollisionManager m_colliderManager;
	Vec2 m_gravity;

	void integrateAcceleration(Rigidbody& rigidbody, Transform& transform);
	void applyForces(Rigidbody& rigidbody);

public:
	PhysicsSystem();
	~PhysicsSystem() = default;

	void init();
	void update() override;
	void receive(Event* ev) override;

	void setGravity(const Vec2& gravity);
	const Vec2& getGravity() const;
};