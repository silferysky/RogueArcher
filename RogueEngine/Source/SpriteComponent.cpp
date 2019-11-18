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
	}

	std::string_view SpriteComponent::getTexturePath() const
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

	void SpriteComponent::DisplayOnInspector()
	{
		const std::string m_constSpritePath = "Resources/Assets/";
		static char m_newSpritePath[128];
		static char m_priorityDraw[128];

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


		ImGui::TextWrapped("Current Draw Priority : %d", m_drawPriority);
		ImGui::TextWrapped("Set Draw Priority");
		ImGui::InputText("                       ", m_priorityDraw, 128);
		if (ImGui::Button("Set Priority"))
		{
			m_drawPriority = atoi(m_priorityDraw);
			setDrawPriority(m_drawPriority);
			memset(m_priorityDraw, 0, 128);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Higher number means the object will be drawn infront");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Drag Drop Sprite here"))
		{			
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Sprite"))
			{
				std::string payload_n = *(std::string*)payload->Data;
				m_texturePath = payload_n.c_str();
				setTexture(m_texturePath.c_str());
				std::cout << payload_n.c_str() << std::endl;
			}
			ImGui::EndDragDropTarget();
		}
	}

	std::string SpriteComponent::Serialize()
	{
		//std::map<std::string, Texture> textureMap = g_engine.m_coordinator.GetTextureManager().getTextureMap();
		std::ostringstream strstream;
		
		strstream << getTexturePath() << ";";
		strstream << m_drawPriority << ";";
		strstream << m_filter.r << ";" << m_filter.g << ";" << m_filter.b << ";" << m_filter.a;

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

		std::getline(strstream, stdstr, ';');

		setTexturePath(stdstr);
		setTexture(m_texturePath.c_str());

		std::getline(strstream, stdstr, ';');
		m_drawPriority = std::stoi(stdstr);

		std::getline(strstream, stdstr, ';');
		m_filter.r = std::stof(stdstr);

		std::getline(strstream, stdstr, ';');
		m_filter.g = std::stof(stdstr);

		std::getline(strstream, stdstr, ';');
		m_filter.b = std::stof(stdstr);

		std::getline(strstream, stdstr, ';');
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