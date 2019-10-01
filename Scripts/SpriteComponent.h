#pragma once
#include "shader.h"
#include "BasicIO.h"
#include "SOIL.h"
#include "../Physics/TransformComponent.h"
#include "../TextureManager.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class SpriteComponent
{
	static constexpr unsigned int quadIndices[6] =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLuint m_texture;
	GLuint m_shader;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
public:
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	void setShader(std::string vShader, std::string fShader);

	void draw(TransformComponent* transform) const;

	GLuint getTexture() const;
	GLuint getShader() const;
	GLuint getVAO() const;
	GLuint getVBO() const;
	GLuint getEBO() const;
};