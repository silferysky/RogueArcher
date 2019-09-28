#include "REEngine.h"

void REEngine::init()
{
	m_coordinator.Init();

	// Register all systems and init them. System signatures will be set in their respective inits.
	auto pPhysicsSystem = m_coordinator.RegisterSystem<PhysicsSystem>();
//	auto gGraphicsSystem = m_coordinator.RegisterSystem<GraphicsSystem>();
//	auto sm = m_coordinator.RegisterSystem<SceneManager>();
//  etc...
	
	m_coordinator.initSystems();
}

void REEngine::update()
{
	m_coordinator.update();
}
