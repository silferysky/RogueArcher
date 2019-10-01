#include "SpriteComponent.h"
#include "REEngine.h"

SpriteComponent::SpriteComponent()
{
	m_texture = gEngine.m_coordinator.loadTexture("test.bmp");

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::string vertexShader = BasicIO::ReadFile("vertexShader.txt");
	std::string fragmentShader = BasicIO::ReadFile("fragmentShader.txt");

	m_shader = CreateShader(vertexShader, fragmentShader);

	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
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

void SpriteComponent::draw(TransformComponent* transform) const
{
	float left = transform->getPosition().x - transform->getScale().x;
	float right = transform->getPosition().x + transform->getScale().x;

	float top = transform->getPosition().y + transform->getScale().y;
	float bottom = transform->getPosition().y - transform->getScale().y;

	float _vertexpos[] =
	/* {
		// positions          // colors           // texture coords
		right,  top, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		right, bottom, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	   left, bottom, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	   left,  top, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	}; */

	{
	// positions          // colors           // texture coords
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);//Reset
	glBindVertexArray(0);//Reset

	//draw
	glBindVertexArray(m_VAO);

	// Use the shader program for drawing
	glUseProgram(m_shader);

	// Draw the Mesh
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// Unbind after drawing
	glBindVertexArray(0);
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