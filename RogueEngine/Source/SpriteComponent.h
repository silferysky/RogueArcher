#pragma once
#include "TextureManager.h"
#include "REEngine.h"

class SpriteComponent
{
	GLuint m_texture;
public:
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	GLuint getTexture() const;
};