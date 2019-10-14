#include "REEngine.h"
#include "LogicSystem.h"

bool REEngine::InitializeOpenGL()
{
	return true;
}

void REEngine::RegisterSystems()
{
	m_coordinator.RegisterSystem<InputManager>();
	m_coordinator.RegisterSystem<PhysicsSystem>();
	m_coordinator.RegisterSystem<WindowSystem>();
	m_coordinator.RegisterSystem<GraphicsSystem>();
	m_coordinator.RegisterSystem<DebugDrawSystem>();
	//m_coordinator.RegisterSystem<LogicSystem>();
}

void REEngine::RegisterComponents()
{
	m_coordinator.RegisterComponent<SpriteComponent>();
	m_coordinator.RegisterComponent<RigidbodyComponent>();
	m_coordinator.RegisterComponent<TransformComponent>();
	m_coordinator.RegisterComponent<CircleCollider2DComponent>();
	m_coordinator.RegisterComponent<BoxCollider2DComponent>();
	m_coordinator.RegisterComponent<PlayerControllerComponent>();
	//m_coordinator.RegisterComponent<LogicComponent>();
}

void REEngine::init()
{
	// Init OpenGL libraries.
	assert(InitializeOpenGL());
	// Register all systems.
	RegisterSystems();
	
	// Register all components
	RegisterComponents();
	
	// Init systems and system signatures will be set in their respective inits.
	// Other systems and managers will also be initialized here.
	m_coordinator.Init();
}

void REEngine::update()
{	
	m_stepCount = 0;

	m_coordinator.Update();
}

void REEngine::shutdown()
{
	//put graphics shutdown here
}

float REEngine::GetAccumulatedTime() const
{
	return m_accumulatedTime;
}

int REEngine::GetStepCount() const
{
	return m_stepCount;
}
