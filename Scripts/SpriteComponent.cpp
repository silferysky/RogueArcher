#include "SpriteComponent.h"

SpriteComponent::SpriteComponent()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	/* float left = transform.getPosition().x - transform.getScale().x;
	float right = transform.getPosition().x + transform.getScale().x;

	float top = transform.getPosition().y - transform.getScale().y;
	float bottom = transform.getPosition().y + transform.getScale().y; */

	float _vertexpos[] =
	{
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertexpos), _vertexpos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
	glBindVertexArray(0); //Reset

	m_texture = SOIL_load_OGL_texture
	(
		"../../test.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	std::string vertexShader = EngineIO::ReadFile("vertexShader.txt");

	std::string fragmentShader = EngineIO::ReadFile("fragmentShader.txt");

	m_shader = CreateShader(vertexShader, fragmentShader);
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
	std::string vertexShader = EngineIO::ReadFile(vShader);

	std::string fragmentShader = EngineIO::ReadFile(fShader);

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