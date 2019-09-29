#pragma once
#include "shader.h"
#include "EngineIO.h"
#include "SOIL.h"
#include "../Scripts/Quad.h"

class SpriteComponent
{
public:
	GLuint m_texture = 0;
	unsigned int m_shader = 0;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	void setShader(std::string vShader, std::string fShader);

	GLuint getTexture() const;
	unsigned int getShader() const;
	GLuint getVAO() const;
	GLuint getVBO() const;
	GLuint getEBO() const;
};