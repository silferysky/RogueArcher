#pragma once
#include "../Scripts/Main.h"
#include "../Scripts/REMath.h"
#include "../Scripts/EventListener.h"
#include "CollisionManager.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "BoxCollider2D.h"

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