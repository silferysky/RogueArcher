/* Start Header ************************************************************************/
/*!
\file           SpriteComponent.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for SpriteComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
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

	void SpriteComponent::setTexturePath(std::string_view texturePath)
	{
		m_texturePath.assign(texturePath);
		setTexture(texturePath.data());
	}

	std::string_view SpriteComponent::getTexturePath() const
	{
		return m_texturePath;
	}

	void SpriteComponent::setTexCoordMinX(const float& TexCoordMin)
	{
		m_texCoordMinX = TexCoordMin;
	}

	void SpriteComponent::setTexCoordMaxX(const float& TexCoordMax)
	{
		m_texCoordMaxX = TexCoordMax;
	}

	float SpriteComponent::getTexCoordMinX() const
	{
		return m_texCoordMinX;
	}

	float SpriteComponent::getTexCoordMaxX() const
	{
		return m_texCoordMaxX;
	}

	void SpriteComponent::setTexCoordMinY(const float& TexCoordMinY)
	{
		m_texCoordMinY = TexCoordMinY;
	}

	float SpriteComponent::getTexCoordMinY() const
	{
		return m_texCoordMinY;
	}

	void SpriteComponent::setTexCoordMaxY(const float& texCoordMaxY)
	{
		m_texCoordMaxY = texCoordMaxY;
	}

	float SpriteComponent::getTexCoordMaxY() const
	{
		return m_texCoordMaxY;
	}

	void SpriteComponent::setTexCoordScaleX(const float& texCoordScaleX)
	{
		m_texCoordScale.x = texCoordScaleX;
	}

	void SpriteComponent::setTexCoordScaleY(const float& texCoordScaleY)
	{
		m_texCoordScale.x = texCoordScaleY;
	}

	Vec2 SpriteComponent::getTexCoordScale() const
	{
		return m_texCoordScale;
	}

	void SpriteComponent::setTexCoordOffsetX(const float& texCoordOffsetX)
	{
		m_texCoordOffset.x = texCoordOffsetX;
	}

	void SpriteComponent::setTexCoordOffsetY(const float& texCoordOffsetY)
	{
		m_texCoordOffset.y = texCoordOffsetY;
	}

	Vec2 SpriteComponent::getTexCoordOffset() const
	{
		return m_texCoordOffset;
	}

	glm::vec4& SpriteComponent::getFilter()
	{
		return m_filter;
	} 

	void SpriteComponent::setFilter(const glm::vec4& filter)
	{
		m_filter = filter;
	}

	void SpriteComponent::DisplayOnInspector()
	{
		const std::string m_constSpritePath = "Resources/Assets/";
		static char m_newSpritePath[128];
		static char m_priorityDraw[128];

		ImGui::Checkbox("Component Active", &m_componentIsActive);

		ImGui::PushItemWidth(75);
		ImGui::TextWrapped("Current File Path");
		ImGui::TextWrapped("%s", m_texturePath.c_str());
		ImGui::TextWrapped("New Texture Path");
		ImGui::SameLine();
		ImGui::PushItemWidth(200);
		ImGui::InputText("                      ", m_newSpritePath, 128);

		if (ImGui::Button("Set New Sprite"))
		{
			m_texturePath = m_constSpritePath + m_newSpritePath;
			setTexture(m_texturePath.c_str());
			memset(m_newSpritePath, 0, 128);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Enter name of file and click on Set New Path to update sprite. Note that new sprite will only be loaded when scene is saved and restarted");
			ImGui::EndTooltip();
		}

		ImGui::PushItemWidth(250);
		ImGui::ColorEdit4("Color", (float*)& m_filter);

		ImGui::Text("Drag Drop Sprite here");

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Data"))
			{
				DirectoryInfo payload_n = *(DirectoryInfo*)payload->Data;
				if (payload_n.m_fileType == "png" || payload_n.m_fileType == "bmp")
				{
					m_texturePath = payload_n.m_filePath.c_str();
					setTexture(m_texturePath.c_str());
					ImGui::EndDragDropTarget();
				}

				else
				{
					ImGui::OpenPopup("Texture Error");
				}
			}
		}
		bool open = true;
		if (ImGui::BeginPopupModal("Texture Error", &open))
		{
			ImGui::Text("Error!, Please only put in texture files!");
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}

	std::string SpriteComponent::Serialize()
	{
		//std::map<std::string, Texture> textureMap = g_engine.m_coordinator.GetTextureManager().getTextureMap();
		std::ostringstream strstream;
		
		strstream << getTexturePath() << ";";
		strstream << m_filter.r << ";" << m_filter.g << ";" << m_filter.b << ";" << m_filter.a << ";";

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

	void SpriteComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream strstream(toDeserialize.data());
		std::string stdstr;

		if (std::getline(strstream, stdstr, ';'))
		{
			setTexturePath(stdstr);
			//setTexture(m_texturePath.c_str());
		}

		if (std::getline(strstream, stdstr, ';'))
			m_filter.r = std::stof(stdstr);

		if (std::getline(strstream, stdstr, ';'))
			m_filter.g = std::stof(stdstr);

		if (std::getline(strstream, stdstr, ';'))
			m_filter.b = std::stof(stdstr);

		if (std::getline(strstream, stdstr, ';'))
			m_filter.a = std::stof(stdstr);
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