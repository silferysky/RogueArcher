#include "SpriteComponent.h"
#include "REEngine.h"

SpriteComponent::SpriteComponent() : m_effectMat{ 1.0 }
{
	// create this once
	m_texture = gEngine.m_coordinator.loadTexture("CharaTest.bmp");

	std::string vertexShader = BasicIO::ReadFile("vertexShader.txt");
	std::string fragmentShader = BasicIO::ReadFile("fragmentShader.txt");

	m_shader = CreateShader(vertexShader, fragmentShader);
}

void SpriteComponent::setTexture(const char* texture)
{
	m_texture = gEngine.m_coordinator.loadTexture(texture);
}

void SpriteComponent::draw(TransformComponent* transform)
{
	{
		auto transformMat = glm::mat4(1.0f);

		transformMat = glm::translate(transformMat, { transform->getPosition().x, transform->getPosition().y, 1.0f});
		transformMat = glm::scale(transformMat, glm::vec3(transform->getScale().x, transform->getScale().y, 1.0f));
		transformMat = glm::rotate(transformMat, transform->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
		
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//draw
		 // Use the shader program for drawing
		// model to world, world to view, view to projection

		//offset by translation of camera, inverse of rotation

		glUseProgram(m_shader);

		m_effectMat = projMat * transformMat;

		GLint effectLocation = glGetUniformLocation(m_shader, "effect");
		glUniformMatrix4fv(effectLocation, 1, GL_FALSE, glm::value_ptr(m_effectMat));

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
		glBindVertexArray(0); //Reset
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