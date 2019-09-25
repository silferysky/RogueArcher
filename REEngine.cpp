#include "REEngine.h"

void REEngine::init()
{
	// Allocate memory for manager objects
	RECoordinator.Init();

	// Register all components
	RECoordinator.RegisterComponent<SpriteComponent>();
	RECoordinator.RegisterComponent<Rigidbody>(); // TODO: Rename to RigidbodyComponent
	RECoordinator.RegisterComponent<Transform>(); // TODO: ^
//	RECoordinator.RegisterComponent<AABB>(); // Will combine AABB and OBB into BoxCollder2DComponent
//	RECoordinator.RegisterComponent<OBB>();
	RECoordinator.RegisterComponent<CircleCollider2D>();

	// Register all systems
//	RECoordinator.RegisterSystem<GameStateManager();
//	RECoordinator.RegisterSystem<GraphicsSystem>();
	RECoordinator.RegisterSystem<PhysicsSystem>();
//	RECoordinator.RegisterSystem<SceneManager>();
//  etc...

}

void REEngine::update()
{

}
