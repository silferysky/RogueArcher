/*Start Header*************************************************************************/
/*!
\file           DebugDrawSystem.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for DebugDrawSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
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
#include "KeyEvent.h"
#include "GameEvent.h"

//namespace Rogue
//{
	DebugDrawSystem::DebugDrawSystem() :
		m_EBO{ 0 }, m_VAO{ 0 }, m_VBO{ 0 }, m_shader{ Rogue::Shader() },
		System(Rogue::SystemID::id_DEBUGDRAWSYSTEM) {}

	// Public member functions
	void DebugDrawSystem::Init()
	{
		Rogue::LISTENER_HANDLER hand = std::bind(&DebugDrawSystem::Receive, this, std::placeholders::_1);
		Rogue::EventDispatcher::instance().AddListener(Rogue::SystemID::id_DEBUGDRAWSYSTEM, hand);

		// Add components to signature
		Rogue::Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<Rogue::ColliderComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<Rogue::RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<Rogue::TransformComponent>());
		
		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<DebugDrawSystem>(signature);

		m_shader = g_engine.m_coordinator.loadShader("Debug Shader");

		glUseProgram(m_shader.GetShader());

		GLint m_uniformBlockIndex = glGetUniformBlockIndex(m_shader.GetShader(), "Matrices");
		glUniformBlockBinding(m_shader.GetShader(), m_uniformBlockIndex, 0);
		m_uboMatrices = g_engine.m_coordinator.GetSystem<Rogue::GraphicsSystem>()->getUBOMatrices();

		m_filterLocation = glGetUniformLocation(m_shader.GetShader(), "colourFilter");

		Rogue::GenerateLinePrimitive(m_VBO, m_VAO);

		m_pCamera = g_engine.m_coordinator.GetSystem<Rogue::CameraSystem>();
	}

	void DebugDrawSystem::Update()
	{
		// Fake update. Timer will be inaccurate for debug draw
	}

	void DebugDrawSystem::TrueUpdate()
	{
		glUseProgram(m_shader.GetShader());

		glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(g_engine.GetProjMat()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_pCamera->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// For all entities
		for (auto entity : m_entities)
		{
			if (!m_isActive)
				break;

			if (g_engine.m_coordinator.ComponentExists<Rogue::UIComponent>(entity))
				continue;

			auto& transform = g_engine.m_coordinator.GetComponent<Rogue::TransformComponent>(entity);
			auto& rBody = g_engine.m_coordinator.GetComponent<Rogue::RigidbodyComponent>(entity);

			//glDisable(GL_DEPTH_TEST);

			if (entity) // If not background
			{
				if (auto optionalCollider = g_engine.m_coordinator.TryGetComponent<Rogue::BoxCollider2DComponent>(entity))
				{
					Rogue::BoxCollider2DComponent& box = optionalCollider->get();

					if (box.GetIsCollided())
					{
						if (box.GetCollisionMode() == Rogue::CollisionMode::e_trigger)
							glUniform4fv(m_filterLocation, 1, glm::value_ptr(glm::vec4(0.196f, 0.804f, 0.196f, 1.0f))); // lime green
						else
							glUniform4fv(m_filterLocation, 1, glm::value_ptr(glm::vec4(1.0f, 0.411f, 0.705f, 1.0f))); // hot pink
					}
					else
					{
						if (box.GetCollisionMode() == Rogue::CollisionMode::e_trigger)
							glUniform4fv(m_filterLocation, 1, glm::value_ptr(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f))); // cyan
						else
							glUniform4fv(m_filterLocation, 1, glm::value_ptr(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f))); // orange
					}

					drawAABB(&box, &transform);
					//drawOBB(&box, &rBody);
				}
				if (auto optionalCollider = g_engine.m_coordinator.TryGetComponent<Rogue::CircleCollider2DComponent>(entity))
				{
					Rogue::CircleCollider2DComponent& circle = optionalCollider->get();

					if (circle.GetIsCollided())
					{
						glUniform4fv(m_filterLocation, 1, glm::value_ptr(glm::vec4(1.0f, 0.411f, 0.705f, 1.0f)));
					}
					else
					{
						glUniform4fv(m_filterLocation, 1, glm::value_ptr(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)));
					}

					drawCircle(&circle, &transform);
				}
				
				drawVelocity(&rBody, &transform);
			}
		}

		glUseProgram(0);
	}

	void DebugDrawSystem::drawAABB(Rogue::BoxCollider2DComponent* box, Rogue::TransformComponent* transform)
	{
		//std::cout << "Drawing AABB" << std::endl;

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

		//std::cout << "Drawing OBB" << std::endl;
		// TODO: Move OBB to another component.
#if 0
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		auto& obb = box->m_obb;

		if (obb.getSize() == 0)
			return;

		for (size_t i = 0; i < obb.getSize() - 1; ++i)
		{
			drawLine(obb.globVerts()[i], obb.globVerts()[i + 1]);
		}

		drawLine(obb.globVerts()[obb.getSize() - 1], obb.globVerts()[0]);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
#endif
	}

	void DebugDrawSystem::drawCircle(Rogue::CircleCollider2DComponent* circle, Rogue::TransformComponent* transform)
	{
		//std::cout << "Drawing Circle" << std::endl;

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		const float segments = 32.0f;
		const float increment = 2.0f * Rogue::RE_PI / segments;

		float radius = circle->m_collider.getRadius();
		Rogue::Vec2 center = circle->m_collider.GetCenter();

		float sinInc = sinf(increment);
		float cosInc = cosf(increment);
		Rogue::Vec2 r1(1.0f, 0.0f);
		Rogue::Vec2 v1 = { center.x + radius * r1.x, center.y + radius * r1.y};

		for (int i = 0; i < segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			Rogue::Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			Rogue::Vec2 v2 = { center.x + radius * r2.x, center.y + radius * r2.y};
			drawLine(v1, v2);

			r1 = r2;
			v1 = v2;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDrawSystem::drawVelocity(Rogue::RigidbodyComponent* rBody, Rogue::TransformComponent* transform)
	{
		if (rBody->getIsStatic())
			return;

		//std::cout << "Drawing Velocity" << std::endl;
		
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		float posX = transform->GetPosition().x;
		float posY = transform->GetPosition().y;

		drawLine(Rogue::Vec2(posX, posY), Rogue::Vec2(posX + rBody->getVelocity().x * 0.3f,
													  posY + rBody->getVelocity().y * 0.3f));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDrawSystem::Receive(Rogue::Event& ev)
	{
		switch (ev.GetEventType())
		{
		case Rogue::EventType::EvKeyTriggered:
		{
			Rogue::KeyTriggeredEvent& keytriggeredevent = dynamic_cast<Rogue::KeyTriggeredEvent&>(ev);
			Rogue::KeyPress keycode = keytriggeredevent.GetKeyCode();
		    
			if (g_engine.GetIsFocused())
			{
				if (keycode == Rogue::KeyPress::KeyF3)
					m_isActive = !m_isActive;
			}
			return;
		} //End KeyTriggered
		}
	}

	void DebugDrawSystem::Shutdown()
	{}
//}