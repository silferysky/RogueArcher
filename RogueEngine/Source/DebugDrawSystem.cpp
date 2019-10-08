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

	GenerateQuadPrimitive(m_VBO, m_VAO, m_EBO);
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

		if (entity)
			drawDebug(&collider, &transform);
	}
	TimeSystem.TimerEnd("Graphics System");
}

void DebugDrawSystem::drawDebug(BoxCollider2DComponent* box, TransformComponent* transform)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(m_VAO);

	auto transformMat = glm::mat4(1.0f);

	float left = box->AABB().getMin().x;
	float right = box->AABB().getMax().x;

	float top = box->AABB().getMax().y;
	float bottom = box->AABB().getMin().y;

	transformMat = glm::translate(transformMat, { (left + right) * 0.5f, (top + bottom) * 0.5f, 1.0f });
	transformMat = glm::scale(transformMat, glm::vec3(transform->getScale().x, transform->getScale().x, 1.0f));

	glBindVertexArray(m_VAO);

	glUseProgram(m_shader.GetShader());

	transformMat = projMat * transformMat;

	GLint transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
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