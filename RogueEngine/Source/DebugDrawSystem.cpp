#include "REEngine.h"
#include "Timer.h"
#include "Main.h"
#include "REMath.h"
#include "EventDispatcher.h"
#include "GLHelper.hpp"
#include "ComponentList.h"
#include "DebugDrawSystem.h"	
#include "SystemManager.h"
#include "EventListener.h"

DebugDrawSystem::DebugDrawSystem()
	:System(SystemID::id_DEBUGDRAWSYSTEM){}

// Public member functions
void DebugDrawSystem::init()
{
	LISTENER_HANDLER hand = std::bind(&DebugDrawSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_DEBUGDRAWSYSTEM, hand);

	// Add components to signature
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<RigidbodyComponent>());

	// Set graphics system signature
	gEngine.m_coordinator.SetSystemSignature<DebugDrawSystem>(signature);

	m_shader = gEngine.m_coordinator.loadShader("Debug Shader");

	glUseProgram(m_shader.GetShader());

	GLint transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(projMat));

	GenerateLinePrimitive(m_VBO, m_VAO);
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
		auto& rBody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);

		glDisable(GL_DEPTH_TEST);

		if (entity)
		{
			drawAABB(&collider, &transform);
			drawOBB(&collider);
			drawVelocity(&rBody, &transform);
		}
	}
	TimeSystem.TimerEnd("Graphics System");
}

void DebugDrawSystem::drawAABB(BoxCollider2DComponent* box, TransformComponent* transform)
{
	glUseProgram(m_shader.GetShader());
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	float left = box->AABB().getMin().x;
	float right = box->AABB().getMax().x;

	float top = box->AABB().getMax().y;
	float bottom = box->AABB().getMin().y;

	drawLine(Vec2(left, top), Vec2(right, top)); // top line
	drawLine(Vec2(left, bottom), Vec2(right, bottom)); // bottom line
	drawLine(Vec2(left, top), Vec2(left, bottom)); // left line
	drawLine(Vec2(right, top), Vec2(right, bottom)); // right line

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void DebugDrawSystem::drawOBB(BoxCollider2DComponent* box)
{
	glUseProgram(m_shader.GetShader());
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	auto obb = box->OBB();
	for (unsigned int i = 0; i < obb.getSize() - 1; ++i)
	{
		drawLine(obb.globVerts()[i], obb.globVerts()[i + 1]);
	}

	drawLine(obb.globVerts()[obb.getSize() - 1], obb.globVerts()[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void DebugDrawSystem::drawVelocity(RigidbodyComponent* rBody, TransformComponent* transform)
{
	glUseProgram(m_shader.GetShader());
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	float posX = transform->getPosition().x;
	float posY = transform->getPosition().y;

	drawLine(Vec2(posX, posY), Vec2(posX + rBody->getVelocity().x, posY + rBody->getVelocity().y));

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