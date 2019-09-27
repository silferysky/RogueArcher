#include "REEngine.h"

void REEngine::init()
{
	// Allocate memory for manager objects
	m_coordinator.Init();

	// Register all components
	m_coordinator.RegisterComponent<SpriteComponent>();
	m_coordinator.RegisterComponent<Rigidbody>(); 
	m_coordinator.RegisterComponent<Transform>(); 
	m_coordinator.RegisterComponent<CircleCollider2D>();
	m_coordinator.RegisterComponent<BoxCollider2D>();

	// Register all systems and init them. System signatures will be set in their respective inits.
	auto gPhysicsSystem = m_coordinator.RegisterSystem<PhysicsSystem>();
	gPhysicsSystem->init();

//	auto gGraphicsSystem = m_coordinator.RegisterSystem<GraphicsSystem>();
//	auto sm = m_coordinator.RegisterSystem<SceneManager>();
//  etc...
}

void REEngine::update()
{
	
}
