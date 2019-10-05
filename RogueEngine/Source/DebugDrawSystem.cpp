#include "DebugDrawSystem.h"

// Public member functions 
void DebugDrawSystem::init()
{
	LISTENER_HANDLER hand = std::bind(&DebugDrawSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_DEBUGDRAWSYSTEM, hand);

	// Add components to signature
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());

	// Set graphics system signature
	gEngine.m_coordinator.SetSystemSignature<DebugDrawSystem>(signature);

	m_shader = gEngine.m_coordinator.loadShader("Debug Shader");

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
}

void DebugDrawSystem::update()
{
	Timer TimeSystem;
	TimeSystem.TimerInit("Debug Draw System");

	// For all entities
	for (auto entity : m_entities)
	{
		auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
		auto& collider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);

		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(m_VAO);

		if (entity)
			drawDebug(&collider, &transform);

		// Unbind VAO after drawing
		glBindVertexArray(0);
	}
	TimeSystem.TimerEnd("Graphics System");
}

void DebugDrawSystem::drawDebug(BoxCollider2DComponent* box, TransformComponent* transform)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto transformMat = glm::mat4(1.0f);

	float left = box->AABB().getMin().x;
	float right = box->AABB().getMax().x;

	float top = box->AABB().getMax().y;
	float bottom = box->AABB().getMin().y;

	transformMat = glm::translate(transformMat, { (left + right) * 0.5f, (top + bottom) * 0.5f, 1.0f });
	transformMat = glm::scale(transformMat, glm::vec3(transform->getScale().x, transform->getScale().x, 1.0f));
	transformMat = glm::rotate(transformMat, transform->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

	glBindVertexArray(m_VAO);

	glUseProgram(m_shader.GetShader());

	transformMat = projMat * transformMat;

	GLint transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DebugDrawSystem::receive(Event* ev)
{
	switch (ev->GetEventType())
	{
	case EventType::EvKeyPressed:
	{
	}
	default:
		return;
	}
}