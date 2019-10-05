#include "SpriteComponent.h"
#include "REEngine.h"

SpriteComponent::SpriteComponent()
{
	// create this once
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