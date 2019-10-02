#pragma once
#include "Coordinator.h"
#include "PhysicsComponent.h"
#include "EntityManager.h"
#include "SystemManager.h"

extern Coordinator RECoordinator;
class PhysicsSystem: public System
{
	void init();
	void Update(float dt);
};
void PhysicsSystem::Update(float dt)
{
	for (auto const& entity : REEntities)
	{
		auto& rigidBody = RECoordinator.GetComponent<RigidBody>(entity);
		auto& transform = RECoordinator.GetComponent<Transform>(entity);
		auto const& gravity = RECoordinator.GetComponent<Gravity>(entity);

		transform.position += rigidBody.velocity * dt;

		rigidBody.velocity += gravity.force * dt;
	}
}