#include "AudioEmitterComponent.h"
#include <sstream>

namespace Rogue
{
	AudioEmitterComponent::~AudioEmitterComponent()
	{
		if (m_sound.GetSystem() != NULL)
			m_sound.Release();
	}

	void AudioEmitterComponent::setID(const std::string& id)
	{
		m_soundPath = id;
	}

	std::string AudioEmitterComponent::getID() const
	{
		return m_soundPath;
	}

	std::string AudioEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_soundPath;
		return ss.str();
	}

	Sound* AudioEmitterComponent::getSound()
	{
		return &m_sound;
	}

	void AudioEmitterComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;			//s2 is used if two are needed
		//int counter = 0;		//Needed to take in for multiple values
		//int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1	parameter to set

		while (std::getline(ss, s1, ';'))
		{
			setID(s1);
		}
	}
}