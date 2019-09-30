#include "REEngine.h"

void REEngine::init()
{
	// Init OpenGL
	glEnable(GL_TEXTURE_2D);						   // Texture Mapping
	glShadeModel(GL_SMOOTH);						   // Smooth shading
	glDepthFunc(GL_LEQUAL);							   // Depth testing type
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Perspective Calculations

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW broke" << std::endl;

	m_coordinator.Init();

}

void REEngine::update()
{
//	RE_CORE_INFO("Engine update");
	m_coordinator.update();
}
