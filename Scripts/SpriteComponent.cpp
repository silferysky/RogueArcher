#include "SpriteComponent.h"

SpriteComponent::SpriteComponent() : m_texture{ 0 }, m_shader{ 0 }
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

void SpriteComponent::setTexture(const char* texture)
{
	m_texture = SOIL_load_OGL_texture
	(
		texture,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
}

void SpriteComponent::setShader(std::string vShader, std::string fShader)
{
	std::string vertexShader = BasicIO::ReadFile("vertexShader.txt");

	std::string fragmentShader = BasicIO::ReadFile("fragmentShader.txt");

	m_shader = CreateShader(vertexShader, fragmentShader);
}

GLuint SpriteComponent::getTexture() const
{
	return m_texture;
}

unsigned int SpriteComponent::getShader() const
{
	return m_shader;
}

unsigned int SpriteComponent::getVAO() const
{
	return m_VAO;
}

unsigned int SpriteComponent::getVBO() const
{
	return m_VBO;
}

unsigned int SpriteComponent::getEBO() const
{
	return m_EBO;
}