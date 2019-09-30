#pragma once
#include "shader.h"
#include "BasicIO.h"
#include "SOIL.h"
#include "../Scripts/Quad.h"
#include "../Physics//Transform.h"

class SpriteComponent
{
	static constexpr unsigned int quadIndices[6] =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLuint m_texture = 0;
	unsigned int m_shader = 0;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
public:
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	void setShader(std::string vShader, std::string fShader);

	void draw(Transform* transform) const;

	GLuint getTexture() const;
	unsigned int getShader() const;
	GLuint getVAO() const;
	GLuint getVBO() const;
	GLuint getEBO() const;
};