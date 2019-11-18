#include "Precompiled.h"
#include "ParticleEmitterComponent.h"
#include "Main.h"

namespace Rogue
{
	void ParticleEmitterComponent::DisplayOnInspector()
	{
		/* static char m_newaudioPath[128];
		const std::string m_constAudioPath = "Resources/Sounds/";
		ImGui::PushItemWidth(75);
		ImGui::TextWrapped("Current Sound Path : ");
		ImGui::TextWrapped("%s", m_soundPath.c_str());
		ImGui::TextDisabled("New Sound Path");
		ImGui::SameLine();
		ImGui::PushItemWidth(250);
		ImGui::InputText("                    ", m_newaudioPath, 128);
		if (ImGui::Button("Set new path"))
		{
			m_soundPath = m_constAudioPath + m_newaudioPath;
			setSoundPath(m_soundPath);
			memset(m_newaudioPath, 0, 128);
		}

		ImGui::DragFloat("Audio Scale", &m_audioScale, 0.01f, 0.0f, 10.0f);
		setAudioScale(m_audioScale);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Enter name of file and click on Set New Path to update sound. Note that new sound will only be loaded when scene is saved and restarted");
			ImGui::EndTooltip();
		} */
	}

	std::string ParticleEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		//ss << m_soundPath << ";";
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
				//setSoundPath(s1);
				break;
			case 1:
				//setAudioScale(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}