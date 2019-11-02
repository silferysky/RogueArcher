#include "AudioEmitterComponent.h"
#include <sstream>
#include "Main.h"

namespace Rogue
{
	AudioEmitterComponent::~AudioEmitterComponent()
	{
		//if (m_sound.GetSystem() != NULL)
			//m_sound.Release();
	}

	void AudioEmitterComponent::setSoundPath(const std::string& soundPath)
	{
		m_soundPath = soundPath;
	}

	std::string AudioEmitterComponent::getSoundPath() const
	{
		return m_soundPath;
	}

	std::string AudioEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_soundPath;
		return ss.str();
	}

	void AudioEmitterComponent::setSound(const std::string& sound)
	{
		//m_sound = g_engine.m_coordinator.loadSound(sound);
	}

	Sound& AudioEmitterComponent::getSound()
	{
		return m_sound;
	}

	void AudioEmitterComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;			//s2 is used if two are needed
		//int counter = 0;		//Needed to take in for multiple values
		//int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1	parameter to set

		while (std::getline(ss, s1, ';'))
		{
			setSoundPath(s1);
		}
	}
}