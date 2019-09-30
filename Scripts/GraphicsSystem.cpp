#include "GraphicsSystem.h"

// Public member functions 

void GraphicsSystem::init()
{
	// Add components to signature
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<SpriteComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2D>());
	signature.set(gEngine.m_coordinator.GetComponentType<Transform>());

	// Set graphics system signature
	gEngine.m_coordinator.SetSystemSignature<GraphicsSystem>(signature);

	glEnable(GL_TEXTURE_2D);						   // Texture Mapping
	glShadeModel(GL_SMOOTH);						   // Smooth shading
	glDepthFunc(GL_LEQUAL);							   // Depth testing type
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Perspective Calculations

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW broke" << std::endl;
}

void GraphicsSystem::update()
{
	// For all entities
	for (auto entity : m_entities)
	{
		auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = gEngine.m_coordinator.GetComponent<Transform>(entity);
		auto& collider = gEngine.m_coordinator.GetComponent<BoxCollider2D>(entity);

		//draw
		glBindVertexArray(sprite.getVAO());

		// Use the shader program for drawing
		glUseProgram(sprite.getShader());

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Unbind after drawing
		glBindVertexArray(0);
	}
}

void GraphicsSystem::receive(Event* ev)
{
	RE_INFO(ev->ToString());
	RE_CORE_INFO("GRAPHICS SYSTEM RECEIVED EVENT");
}