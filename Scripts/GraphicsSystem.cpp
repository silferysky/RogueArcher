#include "GraphicsSystem.h"

// Public member functions 
void GraphicsSystem::init()
{
	LISTENER_HANDLER hand = std::bind(&GraphicsSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_GRAPHICSSYSTEM, hand);

	// Add components to signature
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<SpriteComponent>());
	//signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());

	// Set graphics system signature
	gEngine.m_coordinator.SetSystemSignature<GraphicsSystem>(signature);
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
		//auto& collider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);

		//glDisable(GL_DEPTH_TEST);
		
		sprite.draw(&transform);
		//drawDebug(&collider);
	}
	TimeSystem.TimerEnd("Graphics System");
}

void GraphicsSystem::drawDebug(BoxCollider2DComponent* box)
{
	// GLuint VBO, VAO;

	float left = box->m_aabb.getMin().x;
	float right = box->m_aabb.getMax().x;

	float top = box->m_aabb.getMax().y;
	float bottom = box->m_aabb.getMin().y;

	glBegin(GL_LINES);
	glVertex2f(left, top);
	glVertex2f(right, top);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(right, top);
	glVertex2f(right, bottom);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(left, bottom);
	glVertex2f(right, bottom);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(left, top);
	glVertex2f(left, bottom);
	glEnd();

	auto& obb = box->m_obb;

	for (int i = 0; i < obb.getSize();)
	{
		glBegin(GL_LINES);
		glVertex2f(obb.modelVerts()[i].x, obb.modelVerts()[i].y);
		glVertex2f(obb.modelVerts()[++i].x, obb.modelVerts()[++i].y);
		glEnd();
	}

	/* float vertices[] = { 
		left, top, 
		right, top, 
		left, bottom, 
		right, bottom
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glDrawElements(GL_LINE_STRIP, 4, GL_UNSIGNED_INT, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
	glBindVertexArray(0); //Reset */
}

void GraphicsSystem::receive(Event* ev)
{
	switch (ev->GetEventType())
	{
	case EventType::EvKeyPressed:
	{
		KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
		if (EvPressKey->GetKeyCode() == KeyPress::KeyR)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::rotate_slow(sprite.m_effectMat, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			RE_INFO("Rotated!");
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyE)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::scale_slow(sprite.m_effectMat, glm::vec3(1.1f, 1.1f, 0.0f));
			}
			RE_INFO("Scaled Up!");
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyQ)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::scale_slow(sprite.m_effectMat, glm::vec3(0.9f, 0.9f, 0.0f));
			}
			RE_INFO("Scaled Down!");
		}

		/* else if (EvPressKey->GetKeyCode() == KeyPress::KeyA)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::translate(sprite.m_effectMat, glm::vec3(-0.1f, 0.0f, 0.0f));
			}
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyD)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::translate(sprite.m_effectMat, glm::vec3(0.1f, 0.0f, 0.0f));
			}
		} */

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyW)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::translate(sprite.m_effectMat, glm::vec3(0.0f, 0.1f, 0.0f));
			}
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyS)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::translate(sprite.m_effectMat, glm::vec3(0.0f, -0.1f, 0.0f));
			}
		}

		return;
	}
	default:
		return;
	}
}