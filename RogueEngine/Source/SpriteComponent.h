#pragma once
#include "TextureManager.h"
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