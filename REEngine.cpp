#include "REEngine.h"

void REEngine::init()
{
	// Allocate memory for manager objects
	RECoordinator.Init();

	// Register all components
	RECoordinator.RegisterComponent<SpriteComponent>();
	RECoordinator.RegisterComponent<Rigidbody>(); 
	RECoordinator.RegisterComponent<Transform>(); 
	RECoordinator.RegisterComponent<CircleCollider2D>();
	RECoordinator.RegisterComponent<BoxCollider2D>();

	// Register all systems and init them. System signatures will be set in their respective inits.
	auto gPhysicsSystem = RECoordinator.RegisterSystem<PhysicsSystem>();
	gPhysicsSystem->init();

//	auto gGraphicsSystem = RECoordinator.RegisterSystem<GraphicsSystem>();
//	auto sm = RECoordinator.RegisterSystem<SceneManager>();
//  etc...
}

void REEngine::update()
{
	
}
