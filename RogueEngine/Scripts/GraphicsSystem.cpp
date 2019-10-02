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
	//glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
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
		//auto& collider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);

		//glDisable(GL_DEPTH_TEST);
		
		glBindVertexArray(m_VAO);

		sprite.draw(&transform);
		// Unbind VAO after drawing
		glBindVertexArray(0);

		//drawDebug(&collider);
	}
	TimeSystem.TimerEnd("Graphics System");
}

void GraphicsSystem::drawDebug(BoxCollider2DComponent* box)
{
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

	for (int i = 0; i < static_cast<int>(obb.getSize());)
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
		auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(m_entities.size() - 1);
		KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
		if (EvPressKey->GetKeyCode() == KeyPress::KeyR)
		{
			sprite.m_effectMat = glm::rotate_slow(sprite.m_effectMat, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			RE_INFO("Rotated!");
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyE)
		{
			sprite.m_effectMat = glm::scale_slow(sprite.m_effectMat, glm::vec3(1.1f, 1.1f, 0.0f));
			RE_INFO("Scaled Up!");
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyQ)
		{
			sprite.m_effectMat = glm::scale_slow(sprite.m_effectMat, glm::vec3(0.9f, 0.9f, 0.0f));
			RE_INFO("Scaled Down!");
		}
		return;
	}
	default:
		return;
	}
}