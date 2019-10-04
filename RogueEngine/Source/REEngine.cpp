#include "REEngine.h"

bool REEngine::InitializeOpenGL()
{
	// Init OpenGL
	glEnable(GL_TEXTURE_2D);						   // Texture Mapping
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);						   // Smooth shading
	glDepthFunc(GL_LEQUAL);							   // Depth testing type
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Perspective Calculations

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW broke" << std::endl;
		return false;
	}

	glfwInit();

	return true;
}

void REEngine::RegisterSystems()
{
	m_coordinator.RegisterSystem<PhysicsSystem>();
	m_coordinator.RegisterSystem<GraphicsSystem>();
}

void REEngine::RegisterComponents()
{
	m_coordinator.RegisterComponent<SpriteComponent>();
	m_coordinator.RegisterComponent<RigidbodyComponent>();
	m_coordinator.RegisterComponent<TransformComponent>();
	m_coordinator.RegisterComponent<CircleCollider2DComponent>();
	m_coordinator.RegisterComponent<BoxCollider2DComponent>();
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
	m_coordinator.InitSystems();

	// Init the input manager
	m_inputManager.init();
	
	// Init the event dispatcher
	m_eventDispatcher.init();	
}

void REEngine::update()
{
	m_coordinator.UpdateSystems();
	m_inputManager.update();
	m_eventDispatcher.update();
}