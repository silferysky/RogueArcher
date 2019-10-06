#include "SpriteComponent.h"

SpriteComponent::SpriteComponent()
{
	m_texture = gEngine.m_coordinator.loadTexture("CharaTest.bmp");
}

void SpriteComponent::setTexture(const char* texture)
{
	m_texture = gEngine.m_coordinator.loadTexture(texture);
}

GLuint SpriteComponent::getTexture() const
{
	return m_texture;
}

std::string SpriteComponent::Serialize()
{
	return std::string();
}

void SpriteComponent::Deserialize(std::string toDeserialize)
{
}
