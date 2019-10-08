#pragma once
#include "TextureManager.h"
#include "REEngine.h"

class SpriteComponent
	: public BaseComponent
{
	GLuint m_texture;
public:
	SpriteComponent();
	~SpriteComponent() = default;

	void setTexture(const char* texture);
	GLuint getTexture() const;
	virtual std::string Serialize();
	virtual void Deserialize(std::string toDeserialize);
};