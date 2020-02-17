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

	void LightComponent::setDiffuseFactor(const float& diffuseFactor)
	{
		m_diffuseFactor = diffuseFactor;
	}

	float LightComponent::getDiffuseFactor() const
	{
		return m_diffuseFactor;
	}

	void LightComponent::setSpecularFactor(const float& specularFactor)
	{
		m_specularFactor = specularFactor;
	}

	float LightComponent::getSpecularFactor() const
	{
		return m_specularFactor;
	}

	void LightComponent::setAttenuation(const float& attenuation)
	{
		m_attenuation = attenuation;
	}

	float LightComponent::getAttenuation() const
	{
		return m_attenuation;
	}

	void LightComponent::DisplayOnInspector()
	{
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);

		ImGui::Checkbox("Active?", &m_isActive);
		setIsActive(m_isActive);

		ImGui::DragFloat("Ambient Factor", &m_ambientFactor, 0.1f, 0.0f, 100.0f);
		setAmbientFactor(m_ambientFactor);

		ImGui::DragFloat("Diffuse Factor", &m_diffuseFactor, 0.1f, 0.0f, 100.0f);
		setDiffuseFactor(m_diffuseFactor);

		ImGui::DragFloat("Specular Factor", &m_specularFactor, 0.1f, 0.0f, 100.0f);
		setSpecularFactor(m_specularFactor);

		ImGui::DragFloat("Attenuation", &m_attenuation, 0.1f, 0.0f, 100.0f);
		setAttenuation(m_attenuation);
	}

	std::string LightComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		ss << m_ambientFactor<< ";";
		ss << m_diffuseFactor << ";";
		ss << m_specularFactor << ";";
		ss << m_attenuation << ";";
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
				setIsActive(std::stoi(s1));
				break;
			case 1:
				setAmbientFactor(std::stof(s1));
				break;
			case 2:
				setDiffuseFactor(std::stof(s1));
				break;
			case 3:
				setSpecularFactor(std::stof(s1));
				break;
			case 4:
				setAttenuation(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}