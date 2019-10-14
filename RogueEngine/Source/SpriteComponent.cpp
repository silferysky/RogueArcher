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
	std::map<const char*, GLuint, str_cmp> textureMap = gEngine.m_coordinator.GetTextureManager().getTextureMap();
	
	for (std::map<const char*, GLuint, str_cmp>::iterator it = textureMap.begin(); it != textureMap.end(); ++it)
	{
		if (it->second == m_texture)
		{
			return std::string(it->first);
		}
	}
	return "test.bmp";
}

void SpriteComponent::Deserialize(std::string toDeserialize)
{
	//std::map<const char*, GLuint, str_cmp> textureMap = gEngine.m_coordinator.GetTextureManager().getTextureMap();
	setTexture(toDeserialize.c_str());
}
