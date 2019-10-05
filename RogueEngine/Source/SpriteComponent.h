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
public:
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	GLuint getTexture() const;
};