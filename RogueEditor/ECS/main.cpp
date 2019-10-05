#include "Coordinator.h"
#include <vector>
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

Coordinator RECoordinator;

int main()
{
	RECoordinator.Init();

	RECoordinator.RegisterComponent<Gravity>();
	RECoordinator.RegisterComponent<RigidBody>();
	RECoordinator.RegisterComponent<Transform>();

	auto physicsSystem = RECoordinator.RegisterSystem<PhysicsSystem>();

	Signature signature;
	signature.set(RECoordinator.GetComponentType<Gravity>());
	signature.set(RECoordinator.GetComponentType<RigidBody>());
	signature.set(RECoordinator.GetComponentType<Transform>());
	RECoordinator.SetSystemSignature<PhysicsSystem>(signature);
	//change this when adding entities
	std::vector<Entity> entities(1);


	for (auto& entity : entities)
	{
		entity = RECoordinator.CreateEntity();

		RECoordinator.AddComponent(entity,Gravity{0.0f});
		RECoordinator.AddComponent(entity, RigidBody{ 0.0f,0.0f });
		RECoordinator.RemoveComponent<Gravity>(entity);
	}


	float dt = 0.0f;
	for (auto& entity : entities)
	{
		RECoordinator.DestroyEntity(entity);
	}
}