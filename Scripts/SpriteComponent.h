#pragma once
#include "shader.h"
#include "EngineIO.h"
#include "SOIL.h"

class SpriteComponent
{
	GLuint m_texture = 0;
	unsigned int m_shader = 0;
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;
public:
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	void setShader(std::string vShader, std::string fShader);

	GLuint getTexture() const;
	unsigned int getShader() const;
	unsigned int getVAO() const;
	unsigned int getVBO() const;
	unsigned int getEBO() const;
};