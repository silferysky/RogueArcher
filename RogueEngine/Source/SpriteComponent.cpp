#include "SpriteComponent.h"
#include "REEngine.h"
#include "Main.h"

namespace Rogue
{
	void SpriteComponent::setTexture(const char* texture)
	{
		m_texture = g_engine.m_coordinator.loadTexture(texture);
	}

	Texture& SpriteComponent::getTexture()
	{
		return m_texture;
	}

	void SpriteComponent::setTexturePath(std::string texturePath)
	{
		m_texturePath.assign(texturePath);
	}

	std::string SpriteComponent::getTexturePath() const
	{
		return m_texturePath;
	}

	void SpriteComponent::setDrawPriority(const int& drawPriority)
	{
		m_drawPriority = drawPriority;
	}

	int SpriteComponent::getDrawPriority() const
	{
		return m_drawPriority;
	}

	void SpriteComponent::setTexCoordMin(const float& TexCoordMin)
	{
		m_texCoordMin = TexCoordMin;
	}

	void SpriteComponent::setTexCoordMax(const float& TexCoordMax)
	{
		m_texCoordMax = TexCoordMax;
	}

	float SpriteComponent::getTexCoordMin() const
	{
		return m_texCoordMin;
	}

	float SpriteComponent::getTexCoordMax() const
	{
		return m_texCoordMax;
	}

	glm::vec4& SpriteComponent::getFilter()
	{
		return m_filter;
	}

	void SpriteComponent::setFilter(const glm::vec4& filter)
	{
		m_filter = filter;
	}

	std::string SpriteComponent::Serialize()
	{
		//std::map<std::string, Texture> textureMap = g_engine.m_coordinator.GetTextureManager().getTextureMap();
		std::ostringstream strstream;
		
		strstream << getTexturePath() << ";" << m_drawPriority;

		return strstream.str();
		//Cannot use find because need use value to find key
		/*for (std::map<std::string, Texture>::iterator it = textureMap.begin(); it != textureMap.end(); ++it)
		{
			if (it->second.m_texture == m_texture.m_texture)
			{
				return it->first;
			}
		}*/
	}

	void SpriteComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream strstream(toDeserialize);
		std::string stdstr;

		std::getline(strstream, stdstr, ';');

		setTexturePath(stdstr);
		setTexture(m_texturePath.c_str());

		std::getline(strstream, stdstr, ';');
		m_drawPriority = std::stoi(stdstr);
	}

	/*void SpriteComponent::operator=(SpriteComponent sprite)
	{
		std::map<const char*, GLuint, str_cmp> textureMap = g_engine.m_coordinator.GetTextureManager().getTextureMap();

		for (std::map<const char*, GLuint, str_cmp>::iterator it = textureMap.begin(); it != textureMap.end(); ++it)
		{
			if (it->second == sprite.m_texture)
			{
				setTexture(it->first);
				return;
			}
		}

		setTexture("test.bmp");
	}*/
}