#pragma once
#include "TextureManager.h"
#include "REEngine.h"
#include "ISerializable.h"

class SpriteComponent
	: public ISerializable
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