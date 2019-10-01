#pragma once
#include "shader.h"
#include "BasicIO.h"
#include "SOIL.h"
#include "../Physics/TransformComponent.h"
#include "../TextureManager.h"

class SpriteComponent
{
	static constexpr unsigned int quadIndices[6] =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLuint m_texture = 0;
	GLuint m_shader = 0;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
public:
	SpriteComponent() = default;
	~SpriteComponent() = default;

	SpriteComponent(TransformComponent* transform);

	void setTexture(const char* texture);
	void setShader(std::string vShader, std::string fShader);

	void draw(TransformComponent* transform) const;

	GLuint getTexture() const;
	GLuint getShader() const;
	GLuint getVAO() const;
	GLuint getVBO() const;
	GLuint getEBO() const;
};