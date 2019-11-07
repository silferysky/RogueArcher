#include "AudioEmitterComponent.h"
#include <sstream>
#include "Main.h"

namespace Rogue
{
	void AudioEmitterComponent::Destroy()
	{
		m_sound.Pause(true);

		if (m_sound.GetSystem() != NULL)
			m_sound.Release();
	}

	void AudioEmitterComponent::DisplayOnInspector()
	{
		static char m_newaudioPath[128];
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
		}
	}

	void AudioEmitterComponent::setSoundPath(const std::string& soundPath)
	{
		m_soundPath = soundPath;
	}

	std::string AudioEmitterComponent::getSoundPath() const
	{
		return m_soundPath;
	}

	void AudioEmitterComponent::setSound(const std::string& sound)
	{
		//m_sound = g_engine.m_coordinator.loadSound(sound);
	}

	Sound& AudioEmitterComponent::getSound()
	{
		return m_sound;
	}

	void AudioEmitterComponent::setAudioScale(const float& audioScale)
	{
		m_audioScale = audioScale;
	}

	float& AudioEmitterComponent::getAudioScale()
	{
		return m_audioScale;
	}

	std::string AudioEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_soundPath << ";";
		ss << m_audioScale << ";";
		return ss.str();
	}

	void AudioEmitterComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;			//s2 is used if two are needed
		int counter = 0;		//Needed to take in for multiple values

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				setSoundPath(s1);
				break;
			case 1:
				setAudioScale(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}