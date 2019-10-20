#pragma once
#include "CollisionManager.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "Vector2D.h"

namespace Rogue
{
	class Timer;

	class PhysicsSystem : public System, public EventListener
	{
		Vec2 m_gravity;

		void integrateAcceleration(RigidbodyComponent& rigidbody, TransformComponent& transform);
		void applyForces(RigidbodyComponent& rigidbody);

		// For debugging.
		bool checkAABB;
		bool checkOBB;
		bool allowGravity;
	public:
		PhysicsSystem(Vec2 gravity = { 0.0f, -9.81f });
		~PhysicsSystem() = default;

		void init();
		void update() override;
		void receive(Event* ev) override;

		void setGravity(const Vec2& gravity);
		const Vec2& getGravity() const;
	};
}