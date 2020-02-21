#include "Precompiled.h"
#include "LightComponent.h"
#include "Main.h"

namespace Rogue
{
	void LightComponent::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool LightComponent::getIsActive() const
	{
		return m_isActive;
	}

	void LightComponent::setAmbientFactor(const float& ambientFactor)
	{
		m_ambientFactor = ambientFactor;
	}

	float LightComponent::getAmbientFactor() const
	{
		return m_ambientFactor;
	}

	void LightComponent::setRadius(const float& radius)
	{
		m_radius = radius;
	}

	float LightComponent::getRadius() const
	{
		return m_radius;
	}

	void LightComponent::setSpecularFactor(const float& specularFactor)
	{
		m_specularFactor = specularFactor;
	}

	float LightComponent::getSpecularFactor() const
	{
		return m_specularFactor;
	}

	glm::vec4& LightComponent::getTint()
	{
		return m_tint;
	}

	void LightComponent::setTint(const glm::vec4& tint)
	{
		m_tint = tint;
	}

	void LightComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);

		ImGui::Checkbox("Active?", &m_isActive);
		setIsActive(m_isActive);

		ImGui::DragFloat("Ambient Factor", &m_ambientFactor, 0.1f, 0.0f, 100.0f);
		setAmbientFactor(m_ambientFactor);

		ImGui::DragFloat("Specular Factor", &m_specularFactor, 0.1f, 0.0f, 100.0f);
		setSpecularFactor(m_specularFactor);

		ImGui::DragFloat("Radius", &m_radius, 10.0f, 0.0f, 10000.0f);
		setRadius(m_radius);

		ImGui::PushItemWidth(250);
		ImGui::ColorEdit4("Tint", (float*)&m_tint);
	}

	std::string LightComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		ss << m_ambientFactor<< ";";
		ss << m_radius << ";";
		ss << m_specularFactor << ";";
		ss << m_tint.r << ";" << m_tint.g << ";" << m_tint.b << ";" << m_tint.a << ";";
		return ss.str();
	}

	void LightComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed
		int counter = 0;

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				setIsActive(std::stof(s1));
				break;
			case 1:
				setAmbientFactor(std::stof(s1));
				break;
			case 2:
				setRadius(std::stof(s1));
				break;
			case 3:
				setSpecularFactor(std::stof(s1));
				break;
			case 4:
				//Already gotten new line
				m_tint.r = std::stof(s1);

				if (std::getline(ss, s1, ';'))
					m_tint.g = std::stof(s1);

				if (std::getline(ss, s1, ';'))
					m_tint.b = std::stof(s1);

				if (std::getline(ss, s1, ';'))
					m_tint.a = std::stof(s1);
				break;
			default:
				break;
			}

			++counter;
		}
	}
}