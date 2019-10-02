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

	std::string vertexShader = BasicIO::ReadFile("vertexLineShader.txt");
	std::string fragmentShader = BasicIO::ReadFile("fragmentLineShader.txt");

	m_shader = CreateShader(vertexShader, fragmentShader);

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

	glGenVertexArrays(1, &m_d_VAO);
	glBindVertexArray(m_d_VAO);

	glGenBuffers(1, &m_d_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_d_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_d_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_d_EBO);
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

		sprite.draw(&transform);
		// Unbind VAO after drawing
		glBindVertexArray(0);

		if (entity)
			drawDebug(&collider, &transform);
	}
	TimeSystem.TimerEnd("Graphics System");
}

void GraphicsSystem::drawDebug(BoxCollider2DComponent* box, TransformComponent* transform)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto transformMat = glm::mat4(1.0f);

	float left = box->m_aabb.getMin().x;
	float right = box->m_aabb.getMax().x;

	float top = box->m_aabb.getMax().y;
	float bottom = box->m_aabb.getMin().y;

	transformMat = glm::translate(transformMat, { (left + right) * 0.5f, (top + bottom) * 0.5f, 1.0f });
	transformMat = glm::scale(transformMat, glm::vec3(transform->getScale().x, transform->getScale().x, 1.0f));
	if (transform->getRotation() > 0)
		transformMat = glm::rotate_slow(transformMat, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, transform->getRotation()));

	glUseProgram(m_shader);

	glm::mat4 projMat = glm::ortho(-16.0f * 0.5f, 16.0f * 0.5f, -9.0f * 0.5f, 9.0f * 0.5f, -10.0f, 10.0f);

	glBindVertexArray(m_d_VAO);

	glUseProgram(m_shader);

	projMat = projMat * transformMat;

	GLint effectLocation = glGetUniformLocation(m_shader, "projectionMatrix");
	glUniformMatrix4fv(effectLocation, 1, GL_FALSE, glm::value_ptr(projMat));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsSystem::receive(Event* ev)
{
	switch (ev->GetEventType())
	{
	case EventType::EvKeyPressed:
	{
		auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(static_cast<int>(m_entities.size()) - 1);
		KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
		if (EvPressKey->GetKeyCode() == KeyPress::KeyR)
		{
			transform.offSetRotation(1.0f * gDeltaTime);
			RE_INFO("Rotated!");
		}
		return;
	}
	default:
		return;
	}
}