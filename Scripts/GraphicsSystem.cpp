#include "GraphicsSystem.h"

// Public member functions 

void GraphicsSystem::init()
{
	// Add components to signature
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<SpriteComponent>());
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

void GraphicsSystem::update(float dt)
{
	// For all entities
	for (auto entity : m_entities)
	{
		auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = gEngine.m_coordinator.GetComponent<Transform>(entity);

		glBindVertexArray(sprite.getVAO());

		glBindBuffer(GL_ARRAY_BUFFER, sprite.getVBO());
		//glBufferData(GL_ARRAY_BUFFER, sizeof(_vertexpos), _vertexpos, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.getEBO());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STREAM_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);//Reset
		glBindVertexArray(0);//Reset
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