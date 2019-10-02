#include "REEngine.h"

void REEngine::init()
{
	// Init OpenGL
	glEnable(GL_TEXTURE_2D);						   // Texture Mapping
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);						   // Smooth shading
	glDepthFunc(GL_LEQUAL);							   // Depth testing type
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Perspective Calculations

	// enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW broke" << std::endl;

	glfwInit();

	m_coordinator.Init();
	m_inputManager.init();
	m_eventDispatcher.init();

}

void REEngine::update()
{
//	RE_CORE_INFO("Engine update");
	m_coordinator.update();
	m_inputManager.update();
	m_eventDispatcher.update();
}
