/* Start Header ************************************************************************/
/*!
\file           AudioEmitterComponent.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AudioEmitterComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "AudioEmitterComponent.h"
#include "Precompiled.h"
#include "Main.h"

namespace Rogue
{
	void AudioEmitterComponent::DestroySound()
	{
		m_sound.Pause(true);
	}

	void AudioEmitterComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		float m_volume = getVolume();
		ImGui::DragFloat("Volume", &m_volume, 0.02f, 0.0f, 10.0f);
		setVolume(m_volume);
		m_sound.SetVolume(m_volume);

		bool m_isLooping = getIsLooping();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Audio Looping", &m_isLooping);
		setIsLooping(m_isLooping);
		m_sound.Pause(!m_isLooping);

		bool m_isScaling = getIsScaling();

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Spatial Audio", &m_isScaling);
		setIsScaling(m_isScaling);

		static char m_newaudioPath[128];
		const std::string m_constAudioPath = "Resources/Sounds/";
		ImGui::PushItemWidth(75);
		ImGui::TextWrapped("Current Sound Path : ");
		ImGui::TextWrapped("%s", m_soundPath.c_str());

		ImGui::TextDisabled("New Sound Path");
		ImGui::SameLine();
		ImGui::PushItemWidth(250);
		if (ImGui::InputText("                    ", m_newaudioPath, 128, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_soundPath = m_constAudioPath + m_newaudioPath;
			setSoundPath(m_soundPath);
			memset(m_newaudioPath, 0, 128);
		}

		if (ImGui::Button("Set new path"))
		{
			m_soundPath = m_constAudioPath + m_newaudioPath;
			setSoundPath(m_soundPath);
			memset(m_newaudioPath, 0, 128);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Enter name of file and click on Set New Path to update sound. Note that new sound will only be loaded when scene is saved and restarted");
			ImGui::EndTooltip();
		}

		ImGui::DragFloat("Maximum Distance", &m_maxDistance, 10.0f, 0.0f, 10000.0f);
		setMaxDistance(m_maxDistance);
		m_sound.Set3DMaxDistance(m_maxDistance);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("This sets maximum distance at which the sound can be heard.");
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
		//m_sound = g_engine.m_coordinator.loadSound(getSoundPath());
	}

	std::string AudioEmitterComponent::getSoundPath() const
	{
		return m_soundPath;
	}

	void AudioEmitterComponent::CreateSound()
	{
		m_sound = g_engine.m_coordinator.loadSound(getSoundPath());
	}

	Sound AudioEmitterComponent::getSound()
	{
		return m_sound;
	}

	void AudioEmitterComponent::setIsLooping(bool isLooping)
	{
		m_sound.m_isLooping = isLooping;
	}

	bool& AudioEmitterComponent::getIsLooping()
	{
		return m_sound.m_isLooping;
	}

	void AudioEmitterComponent::setIsScaling(bool isScaling)
	{
		m_isScaling = isScaling;
	}

	bool& AudioEmitterComponent::getIsScaling()
	{
		return m_isScaling;
	}

	void AudioEmitterComponent::setMaxDistance(const float audioScale)
	{
		m_maxDistance = audioScale;
	}

	float& AudioEmitterComponent::getMaxDistance()
	{
		return m_maxDistance;
	}

	void AudioEmitterComponent::setVolume(const float volume)
	{
		m_sound.m_volume = volume;
	}

	float& AudioEmitterComponent::getVolume()
	{
		return m_sound.m_volume;
	}

	std::string AudioEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_soundPath << ";";
		ss << m_maxDistance << ";";
		ss << m_sound.m_volume << ";";
		ss << m_sound.m_isLooping << ";";
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
				setMaxDistance(std::stof(s1));
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

		m_sound = g_engine.m_coordinator.loadSound(getSoundPath(), getVolume(), getIsLooping());
	}
}