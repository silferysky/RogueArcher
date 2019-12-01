#include "Precompiled.h"
#include "Main.h"
#include "ParticleEmitterComponent.h"

namespace Rogue
{
	void ParticleEmitterComponent::DisplayOnInspector()
	{
		bool m_isActive = GetIsActive();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Active?", &m_isActive);
		SetIsActive(m_isActive);

		bool m_isContinuous = GetIsContinuous();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Continuous/Looping?", &m_isContinuous);
		SetIsContinuous(m_isContinuous);

		ImGui::DragFloat("Particle Emitter Magnitude", &m_magnitude, 0.01f, 0.0f, 10.0f);
		SetMagnitude(m_magnitude);

		ImGui::DragFloat("Particle Emitter Arc", &m_arc, 1.0f, 0.0f, 360.0f);
		SetArc(m_arc);

		ImGui::DragFloat("Particle Emitter Angle", &m_angle, 1.0f, 0.0f, 360.0f);
		SetAngle(m_angle);

		ImGui::DragFloat("Particle Max Lifetime", &m_lifetimeLimit, 0.1f, 0.0f, 10.0f);
		SetLifetimeLimit(m_lifetimeLimit);

		ImGui::Text("Particle Scale    ");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat(" ", &m_scale.x, 1.0f, 0.0f, 100000.0f);
		ImGui::SameLine(0.0f, 36.0f);
		ImGui::DragFloat("  ", &m_scale.y, 1.0f, 0.0f, 100000.0f);

		ImGui::Text("Particle Positional Offset Range");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("     ", &m_positionalOffset.x);
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("      ", &m_positionalOffset.y);
		ImGui::PushItemWidth(50);

		ImGui::Text("Particle Spread");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("         ", &m_spread.x);
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("          ", &m_spread.y);
		ImGui::PushItemWidth(50);

		ImGui::Text("Particle Velocity Factor");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("       ", &m_velocityFactor.x);
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("        ", &m_velocityFactor.y);
		ImGui::PushItemWidth(50);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Enter name of file and click on Set New Path to update particle texture.");
			ImGui::EndTooltip();
		}

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

	void ParticleEmitterComponent::SetMagnitude(const float magnitude)
	{
		m_magnitude = magnitude;
	}

	float ParticleEmitterComponent::GetMagnitude() const
	{
		return m_magnitude;
	}

	void ParticleEmitterComponent::SetArc(const float arc)
	{
		m_arc = arc;
	}

	float ParticleEmitterComponent::GetArc() const
	{
		return m_arc;
	}

	void ParticleEmitterComponent::SetAngle(const float angle)
	{
		m_angle = angle;
	}

	float ParticleEmitterComponent::GetAngle() const
	{
		return m_angle;
	}

	void ParticleEmitterComponent::SetLifetimeLimit(const float lifetimeLimit)
	{
		m_lifetimeLimit = lifetimeLimit;
	}

	float ParticleEmitterComponent::GetLifetimeLimit() const
	{
		return m_lifetimeLimit;
	}

	void ParticleEmitterComponent::SetIsFading(const bool isFading)
	{
		m_isFading = isFading;
	}

	bool ParticleEmitterComponent::GetIsFading() const
	{
		return m_isFading;
	}

	void ParticleEmitterComponent::SetScale(const Vec2 scale)
	{
		m_scale = scale;
	}

	Vec2 ParticleEmitterComponent::GetScale() const
	{
		return m_scale;
	}

	void ParticleEmitterComponent::SetVelocity(const Vec2 velocity)
	{
		m_velocityFactor = velocity;
	}

	Vec2 ParticleEmitterComponent::GetVelocity() const
	{
		return m_velocityFactor;
	}

	void ParticleEmitterComponent::SetPositionalOffset(const Vec2 positionalOffset)
	{
		m_positionalOffset = positionalOffset;
	}

	Vec2 ParticleEmitterComponent::GetPositionalOffset() const
	{
		return m_positionalOffset;
	}

	void ParticleEmitterComponent::SetSpread(const Vec2 spread)
	{
		m_spread = spread;
	}

	Vec2 ParticleEmitterComponent::GetSpread() const
	{
		return m_spread;
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
		ss << m_positionalOffset.x << ";" << m_positionalOffset.y << ";";
		ss << m_spread.x << ";" << m_spread.y << ";";
		ss << m_scale.x << ";" << m_scale.y << ";";
		ss << m_velocityFactor.x << ";" << m_velocityFactor.y << ";";;
		ss << m_magnitude << ";";
		ss << m_arc << ";";
		ss << m_angle << ";";
		ss << m_lifetimeLimit << ";";
		ss << m_isFading << ";";
		ss << m_isActive << ";";
		ss << m_isContinuous << ";";
		ss << m_texturePath << ";";

		return ss.str();
	}

	void ParticleEmitterComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1, s2;			//s2 is used if two are needed
		int counter = 0;		//Needed to take in for multiple values
		int sets = 4;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1 parameter to set

		while (std::getline(ss, s1, ';'))
		{
			//If counter has not cleared all sets yet, read into s2
			if (counter < sets)
				std::getline(ss, s2, ';');

			switch (counter)
			{
			case 0:
				SetPositionalOffset(Vec2(std::stof(s1), std::stof(s2)));
			case 1:
				SetSpread(Vec2(std::stof(s1), std::stof(s2)));
			case 2:
				SetScale(Vec2(std::stof(s1), std::stof(s2)));
				break;
			case 3:
				SetVelocity(Vec2(std::stof(s1), std::stof(s2)));
				break;
			case 4:
				SetMagnitude(std::stof(s1));
				break;
			case 5:
				SetArc(std::stof(s1));
				break;
			case 6:
				SetAngle(std::stof(s1));
				break;
			case 7:
				SetLifetimeLimit(std::stof(s1));
				break;
			case 8:
				SetIsFading(std::stof(s1));
				break;
			case 9:
				SetIsActive(std::stof(s1));
				break;
			case 10:
				SetIsContinuous(std::stof(s1));
				break;
			case 11:
				SetTexturePath(s1);
				break;
			default:
				break;
			}

			++counter;
		}
	}
}