#include "GraphicsSystem.h"

// Public member functions 
void GraphicsSystem::init()
{
	LISTENER_HANDLER hand = std::bind(&GraphicsSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_GRAPHICSSYSTEM, hand);

	// Add components to signature
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<SpriteComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());

	// Set graphics system signature
	gEngine.m_coordinator.SetSystemSignature<GraphicsSystem>(signature);

	m_shader = gEngine.m_coordinator.loadShader("Object Shader");

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
	glBindVertexArray(0); //Reset
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsSystem::update()
{
	Timer TimeSystem;
	TimeSystem.TimerInit("Graphics System");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// For all entities
	for (auto entity : m_entities)
	{
		auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
		auto& collider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);

		//glDisable(GL_DEPTH_TEST);
		
		glBindVertexArray(m_VAO);

		draw(&sprite, &transform);

		// Unbind VAO after drawing
		glBindVertexArray(0);
	}
	TimeSystem.TimerEnd("Graphics System");
}

void GraphicsSystem::draw(SpriteComponent* sprite, TransformComponent* transform)
{
	auto transformMat = glm::mat4(1.0f);

	transformMat = glm::translate(transformMat, { transform->getPosition().x, transform->getPosition().y, 1.0f });
	transformMat = glm::scale(transformMat, glm::vec3(transform->getScale().x, transform->getScale().y, 1.0f));
	transformMat = glm::rotate(transformMat, transform->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

	glBindTexture(GL_TEXTURE_2D, sprite->getTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//draw
	// Use the shader program for drawing
	// model to world, world to view, view to projection

	//offset by translation of camera, inverse of rotation

	glUseProgram(m_shader.GetShader());

	transformMat = projMat * transformMat;

	GLint transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

	// Draw the Mesh
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
	glBindVertexArray(0); //Reset
	glUseProgram(0);
}

void GraphicsSystem::receive(Event* ev)
{
	switch (ev->GetEventType())
	{
	case EventType::EvKeyPressed:
	{
		KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
		return;
	}
	default:
		return;
	}
}