#pragma once
#include "shader.h"
#include "BasicIO.h"
#include "SOIL.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "WindowHelper.h"
#include "GLHelper.h"

class SpriteComponent
{
	GLuint m_texture;
	Shader m_shader;
public:
	glm::mat4 m_effectMat;
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	void setShader(std::string vShader, std::string fShader);

	void draw(TransformComponent* transform);

	GLuint getTexture() const;
	GLuint getShader() const;
};