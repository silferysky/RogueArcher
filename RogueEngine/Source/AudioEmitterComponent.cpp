#include "AudioEmitterComponent.h"
#include "Precompiled.h"
#include "Main.h"

namespace Rogue
{
	void AudioEmitterComponent::DestroySound()
	{
		m_sound.Pause(true);

		if (m_sound.GetSystem() != NULL)
			m_sound.Release();
	}

	void AudioEmitterComponent::DisplayOnInspector()
	{
		ImGui::DragFloat("Volume", &m_volume, 0.02f, 0.0f, 10.0f);
		setVolume(m_volume);

		bool m_isLooping = getIsLooping();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Audio Looping", &m_isLooping);
		setIsLooping(m_isLooping);

		bool m_isScaling = getIsScaling();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Audio Scaling", &m_isScaling);
		setIsScaling(m_isScaling);

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

		ImGui::Text("Drag Drop Sound Path Here");

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Data"))
			{
				DirectoryInfo payload_n = *(DirectoryInfo*)payload->Data;
				if (payload_n.m_fileType == "ogg" || payload_n.m_fileType == "wav")
				{
					m_soundPath = payload_n.m_filePath.c_str();
					std::cout << m_soundPath << std::endl;
					setSoundPath(m_soundPath);
					ImGui::EndDragDropTarget();
				}

				else
				{
					ImGui::OpenPopup("Sound Error");
				}
			}
		}
		bool open = true;
		if (ImGui::BeginPopupModal("Sound Error", &open))
		{
			ImGui::Text("Error!, Please only put in sound files!");
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}

	void AudioEmitterComponent::setSoundPath(std::string_view soundPath)
	{
		m_soundPath = soundPath;
	}

	std::string AudioEmitterComponent::getSoundPath() const
	{
		return m_soundPath;
	}

	void AudioEmitterComponent::CreateSound()
	{
		m_sound = g_engine.m_coordinator.loadSound(getSoundPath());

		if (m_isLooping)
		{
			m_sound.CreateBGM(getSoundPath().c_str(), 120.0f);
			m_sound.Play(m_volume);
		}
		else
			m_sound.Create(getSoundPath().c_str(), 12.0f);
	}

	Sound& AudioEmitterComponent::getSound()
	{
		return m_sound;
	}

	void AudioEmitterComponent::setIsLooping(bool isLooping)
	{
		m_isLooping = isLooping;
		m_sound.Pause(!m_isLooping);
	}

	bool& AudioEmitterComponent::getIsLooping()
	{
		return m_isLooping;
	}

	void AudioEmitterComponent::setIsScaling(bool isScaling)
	{
		m_isScaling = isScaling;
	}

	bool& AudioEmitterComponent::getIsScaling()
	{
		return m_isScaling;
	}

	void AudioEmitterComponent::setAudioScale(const float audioScale)
	{
		m_audioScale = audioScale;
	}

	float& AudioEmitterComponent::getAudioScale()
	{
		return m_audioScale;
	}

	void AudioEmitterComponent::setVolume(const float volume)
	{
		m_volume = volume;
		m_sound.SetVolume(m_volume);
	}

	float& AudioEmitterComponent::getVolume()
	{
		return m_volume;
	}

	std::string AudioEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_soundPath << ";";
		ss << m_audioScale << ";";
		ss << m_volume << ";";
		ss << m_isLooping << ";";
		ss << m_isScaling << ";";
		return ss.str();
	}

	void AudioEmitterComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
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
			case 2:
				setVolume(std::stof(s1));
				break;
			case 3:
				setIsLooping(std::stof(s1));
				break;
			case 4:
				setIsScaling(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}