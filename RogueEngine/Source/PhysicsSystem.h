#pragma once
#include "BaseSystem.h"
#include "EventListener.h"
#include "Vector2D.h"
#include "ForceManager.h"

namespace Rogue
{
	class Timer;
	class RigidbodyComponent;
	class TransformComponent;

	class PhysicsSystem : public System, public EventListener
	{
		Vec2 m_gravity;

		void Integrate(RigidbodyComponent& rigidbody, TransformComponent& transform) const;

		// For debugging.
		bool checkAABB;
		bool checkOBB;
		bool allowGravity;
	public:
		PhysicsSystem(Vec2 gravity = { 0.0f, -9.81f });
		~PhysicsSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;

		bool getToggleGravity();
		void setToggleGravity(bool gravity);
		void setGravity(const Vec2& gravity);
		const Vec2& getGravity() const;
	};
}