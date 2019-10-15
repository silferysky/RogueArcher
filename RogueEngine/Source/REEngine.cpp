#include "Logger.h"
#include "InputManager.h"
#include "EventDispatcher.h"
#include "LogicSystem.h"
#include "REEngine.h"
#include "GLHelper.hpp"
#include "PhysicsSystem.h"
#include "GraphicsSystem.h"
#include "DebugDrawSystem.h"
#include "ComponentList.h"
#include "Main.h"

REEngine::REEngine() :
	m_coordinator{}, m_accumulatedTime{ 0.0f }, m_stepCount{ 0 },
	m_gameIsRunning{ true }, firstIter{ true }
{}

bool REEngine::InitializeOpenGL()
{
	// Init OpenGL
	glEnable(GL_TEXTURE_2D);						   // Texture Mapping
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);						   // Smooth shading
	glDepthFunc(GL_LEQUAL);							   // Depth testing type
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Perspective Calculations

	// Enable alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW broke" << std::endl;
		return false;
	}

	return true;
}

void REEngine::RegisterSystems()
{
	m_coordinator.RegisterSystem<InputManager>();
	m_coordinator.RegisterSystem<PhysicsSystem>();
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

void REEngine::GameLoop(HDC hDC)
{
	MSG msg = { 0 };

	m_stepCount = 0;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_loopEnd - m_loopStart).count() / MICRO_TO_SECONDS;

	m_accumulatedTime += gDeltaTime;

	while (m_accumulatedTime >= gFixedDeltaTime)
	{
		m_accumulatedTime -= gFixedDeltaTime;
		m_stepCount++;
	}

	m_coordinator.Update();

	SwapBuffers(hDC);
}

void REEngine::update(HDC hDC)
{	
	m_stepCount = 0;
	std::chrono::high_resolution_clock mainLoopTimer;
	
	while (m_gameIsRunning)
	{
		m_loopStart = mainLoopTimer.now();

		GameLoop(hDC);

		m_loopEnd = mainLoopTimer.now();
	}
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

void REEngine::SetGameIsRunning(bool set)
{
	m_gameIsRunning = set;
}
