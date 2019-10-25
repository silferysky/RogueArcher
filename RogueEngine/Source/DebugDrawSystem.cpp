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

//namespace Rogue
//{
	DebugDrawSystem::DebugDrawSystem() :
		m_EBO{ 0 }, m_VAO{ 0 }, m_VBO{ 0 }, m_shader{ Rogue::Shader() },
		System(Rogue::SystemID::id_DEBUGDRAWSYSTEM) {}

	// Public member functions
	void DebugDrawSystem::init()
	{
		//Rogue::LISTENER_HANDLER hand = std::bind(&receive, this, std::placeholders::_1);
		//Rogue::EventDispatcher::instance().AddListener(Rogue::SystemID::id_DEBUGDRAWSYSTEM, hand);
		//REGISTER_LISTENER(SystemID::id_DEBUGDRAWSYSTEM, DebugDrawSystem::receive);

		// Add components to signature
		Rogue::Signature signature;
		signature.set(g_Engine.m_coordinator.GetComponentType<Rogue::BoxCollider2DComponent>());
		signature.set(g_Engine.m_coordinator.GetComponentType<Rogue::TransformComponent>());
		signature.set(g_Engine.m_coordinator.GetComponentType<Rogue::RigidbodyComponent>());

		// Set graphics system signature
		g_Engine.m_coordinator.SetSystemSignature<DebugDrawSystem>(signature);

		m_shader = g_Engine.m_coordinator.loadShader("Debug Shader");

		glUseProgram(m_shader.GetShader());

		m_transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");

		Rogue::GenerateLinePrimitive(m_VBO, m_VAO);
	}

	void DebugDrawSystem::update()
	{
		g_Engine.m_coordinator.InitTimeSystem("Debug Draw System");

		glUseProgram(m_shader.GetShader());
		glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(g_Engine.GetProjMat()));

		// For all entities
		for (auto entity : m_entities)
		{
			auto& transform = g_Engine.m_coordinator.GetComponent<Rogue::TransformComponent>(entity);
			auto& collider = g_Engine.m_coordinator.GetComponent<Rogue::BoxCollider2DComponent>(entity);
			auto& rBody = g_Engine.m_coordinator.GetComponent<Rogue::RigidbodyComponent>(entity);

			glDisable(GL_DEPTH_TEST);

			if (entity)
			{
				drawAABB(&collider, &transform);
				drawOBB(&collider, &rBody);
				drawVelocity(&rBody, &transform);
			}
		}

		glUseProgram(0);
		g_Engine.m_coordinator.EndTimeSystem("Debug Draw System");
	}

	void DebugDrawSystem::drawAABB(Rogue::BoxCollider2DComponent* box, Rogue::TransformComponent* transform)
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		float left = box->m_aabb.getMin().x;
		float right = box->m_aabb.getMax().x;

		float top = box->m_aabb.getMax().y;
		float bottom = box->m_aabb.getMin().y;

		Rogue::drawLine(Rogue::Vec2(left, top), Rogue::Vec2(right, top)); // top line
		Rogue::drawLine(Rogue::Vec2(left, bottom), Rogue::Vec2(right, bottom)); // bottom line
		Rogue::drawLine(Rogue::Vec2(left, top), Rogue::Vec2(left, bottom)); // left line
		Rogue::drawLine(Rogue::Vec2(right, top), Rogue::Vec2(right, bottom)); // right line

		glBindBuffer(GL_ARRAY_BUFFER, 0);	
		glBindVertexArray(0);
	}

	void DebugDrawSystem::drawOBB(Rogue::BoxCollider2DComponent* box, Rogue::RigidbodyComponent* body)
	{
		if (body->getIsStatic())
			return;

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		auto obb = box->m_obb;
		for (size_t i = 0; i < obb.getSize() - 1; ++i)
		{
			drawLine(obb.globVerts()[i], obb.globVerts()[i + 1]);
		}

		drawLine(obb.globVerts()[obb.getSize() - 1], obb.globVerts()[0]);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDrawSystem::drawVelocity(Rogue::RigidbodyComponent* rBody, Rogue::TransformComponent* transform)
	{
		if (rBody->getIsStatic())
			return;

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		float posX = transform->getPosition().x;
		float posY = transform->getPosition().y;

		drawLine(Rogue::Vec2(posX, posY), Rogue::Vec2(posX + rBody->getVelocity().x * 0.3f,
													  posY + rBody->getVelocity().y * 0.3f));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDrawSystem::receive(Rogue::Event* ev)
	{
		/*switch (ev->GetEventType())
		{
		case EventType::EvKeyPressed:
		{
		}
		default:
			return;
		}*/
	}
//}