#include "Precompiled.h"
#include "ParticleEmitterComponent.h"
#include "Main.h"

namespace Rogue
{
	void ParticleEmitterComponent::DisplayOnInspector()
	{
		const char* particleType[] = { "Explosion", "Spray" };
		int tempInt = (int)m_type;

		//For Particle Type
		ImGui::Combo("Particle Type", &tempInt, particleType, IM_ARRAYSIZE(particleType));
		m_type = (ParticleType)tempInt;

		bool m_isActive = GetIsActive();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Active?", &m_isActive);
		SetIsActive(m_isActive);

		bool m_isContinuous = GetIsContinuous();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Continuous/Looping?", &m_isContinuous);
		SetIsContinuous(m_isContinuous);

		static char m_newtexturePath[128];
		const std::string m_constTexturePath = "Resources/Assets/";
		ImGui::PushItemWidth(75);
		ImGui::TextWrapped("Particle Texture : ");
		ImGui::TextWrapped("%s", m_texturePath.c_str());

		ImGui::TextDisabled("New Particle Texture Path");
		ImGui::SameLine();
		ImGui::PushItemWidth(250);
		ImGui::InputText("                    ", m_newtexturePath, 128);

		if (ImGui::Button("Set new path"))
		{
			m_texturePath = m_constTexturePath + m_newtexturePath;
			SetTexturePath(m_texturePath);
			memset(m_newtexturePath, 0, 128);
		}

		ImGui::DragFloat("Particle Emitter Magnitude", &m_magnitude, 0.01f, 0.0f, 10.0f);
		SetMagnitude(m_magnitude);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Enter name of file and click on Set New Path to update particle texture.");
			ImGui::EndTooltip();
		}

		if (ImGui::Button("Drag Drop Particle Texture Path Here"))
		{

		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Particle Texture"))
			{
				std::string payload_n = *(std::string*)payload->Data;
				m_texturePath = payload_n.c_str();
				SetTexturePath(m_texturePath);
			}
			ImGui::EndDragDropTarget();
		}
	}

	void ParticleEmitterComponent::SetIsActive(const bool isActive)
	{
		m_isActive = isActive;
	}

	bool ParticleEmitterComponent::GetIsActive() const
	{
		return m_isActive;
	}

	void ParticleEmitterComponent::SetIsContinuous(const bool isContinuous)
	{
		m_isContinuous = isContinuous;
	}

	bool ParticleEmitterComponent::GetIsContinuous() const
	{
		return m_isContinuous;
	}

	void ParticleEmitterComponent::SetParticleType(const ParticleType& type)
	{
		m_type = type;
	}

	ParticleType ParticleEmitterComponent::GetParticleType() const
	{
		return m_type;
	}

	void ParticleEmitterComponent::SetMagnitude(const float magnitude)
	{
		m_magnitude = magnitude;
	}

	float ParticleEmitterComponent::GetMagnitude() const
	{
		return m_magnitude;
	}

	void ParticleEmitterComponent::SetTexturePath(std::string_view texturePath)
	{
		m_texturePath.assign(texturePath);
	}

	std::string_view ParticleEmitterComponent::GetTexturePath() const
	{
		return m_texturePath;
	}

	std::string ParticleEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		ss << m_isContinuous << ";";
		ss << static_cast<int>(m_type) << ";";
		ss << m_magnitude << ";";
		ss << m_texturePath << ";";
		return ss.str();
	}

	void ParticleEmitterComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;			//s2 is used if two are needed
		int counter = 0;		//Needed to take in for multiple values

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				SetIsActive(std::stof(s1));
				break;
			case 1:
				SetIsContinuous(std::stof(s1));
				break;
			case 2:
				SetParticleType(static_cast<ParticleType>(stoi(s1)));
				break;
			case 3:
				SetMagnitude(std::stof(s1));
				break;
			case 4:
				SetTexturePath(s1);
				break;
			default:
				break;
			}

			++counter;
		}
	}
}