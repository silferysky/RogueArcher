#include "SpriteComponent.h"
#include "REEngine.h"

SpriteComponent::SpriteComponent() : m_effectMat{ 1.0 }
{
	// create this once
	m_texture = gEngine.m_coordinator.loadTexture("CharaTest.bmp");

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::string vertexShader = BasicIO::ReadFile("vertexShader.txt");
	std::string fragmentShader = BasicIO::ReadFile("fragmentShader.txt");

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
}

void SpriteComponent::setTexture(const char* texture)
{
	m_texture = gEngine.m_coordinator.loadTexture(texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SpriteComponent::setShader(std::string vShader, std::string fShader)
{
	std::string vertexShader = BasicIO::ReadFile(vShader);

	std::string fragmentShader = BasicIO::ReadFile(fShader);

	m_shader = CreateShader(vertexShader, fragmentShader);
}

void SpriteComponent::draw(TransformComponent* transform)
{
	{
		auto transformMat = glm::mat4(1.0f);

		transformMat = glm::scale(transformMat, glm::vec3(transform->getScale().x, transform->getScale().y, 1.0f));
		transformMat = glm::translate(transformMat, { transform->getPosition().x, transform->getPosition().y, 1.0f});

		//draw
		 // Use the shader program for drawing
		// model to world, world to view, view to projection


		//offset by translation of camera, inverse of rotation

		auto projMat = glm::ortho(-9.0f, 9.0f, -6.0f, 6.0f, -10.0f, 10.0f);

		glUseProgram(m_shader);

		GLint projLocation = glGetUniformLocation(m_shader, "projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projMat));

		GLint effectLocation = glGetUniformLocation(m_shader, "effect");
		glUniformMatrix4fv(effectLocation, 1, GL_FALSE, glm::value_ptr(m_effectMat));

		GLint transformLocation = glGetUniformLocation(m_shader, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

		glBindVertexArray(m_VAO);
		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Unbind VAO after drawing

		transformMat = glm::translate(transformMat, { transform->getPosition().x * 3, transform->getPosition().y, 1.0f });
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);
	}
}

GLuint SpriteComponent::getTexture() const
{
	return m_texture;
}

GLuint SpriteComponent::getShader() const
{
	return m_shader;
}

GLuint SpriteComponent::getVAO() const
{
	return m_VAO;
}

GLuint SpriteComponent::getVBO() const
{
	return m_VBO;
}

GLuint SpriteComponent::getEBO() const
{
	return m_EBO;
}