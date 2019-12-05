/* Start Header ************************************************************************/
/*!
\file           AudioManager.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AudioManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include <map>
#include <iostream>
#include "Sound.h"
#include "FMODHelper.h"

namespace Rogue
{
	class AudioManager
	{
		FMOD::System* m_fmodSystem;
		FMOD::DSP* m_DSPLowPassFilter;
		FMOD::DSP* m_DSPHighPassFilter;
		FMOD::Reverb3D* m_reverb;

		std::map<std::string, Sound> audioMap;
		std::string directory = "Resources/Sounds/";
	public:
		static AudioManager& instance()
		{
			static AudioManager instance;
			return instance;
		}

		AudioManager()
		{
			FMOD_RESULT m_result;
			m_result = FMOD::System_Create(&m_fmodSystem);
			FmodErrorCheck(m_result);
			m_result = m_fmodSystem->init(64, FMOD_INIT_NORMAL, 0);
			FmodErrorCheck(m_result);

			// set up the filters
			m_fmodSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &m_DSPLowPassFilter);
			m_DSPLowPassFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 1200.0f);

			m_fmodSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &m_DSPHighPassFilter);
			m_DSPLowPassFilter->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, 1200.0f);

			// set reverb environment to cave
			// TODO: set presets within inspector
			FMOD_REVERB_PROPERTIES reverbProperties = FMOD_PRESET_CAVE;
			m_fmodSystem->createReverb3D(&m_reverb);
			m_reverb->setProperties(&reverbProperties);
			m_reverb->set3DAttributes(0, 0.0f, 10000.0f);
		}
		~AudioManager() = default;

		std::map<std::string, Sound>& getAudioMap()
		{
			return audioMap;
		}

		Sound& loadSound(std::string audio, float volume, bool looping)
		{
			//audio = directory + audio;
			auto itr = audioMap.find(audio);
			if (itr != audioMap.end())
			{
				//itr->second.Play(itr->second.m_volume);
				return itr->second;
			}
			else
			{
				Sound newSound;
				newSound.m_volume = volume;
				newSound.m_isLooping = looping;
				newSound.Create(audio.c_str(), m_fmodSystem, m_DSPLowPassFilter, m_DSPHighPassFilter);
				if (newSound.m_isLooping)
					newSound.Play(newSound.m_volume);
				audioMap.emplace(audio, newSound);
				auto itr1 = audioMap.find(audio);
				return itr1->second;
			}
		}

		void ClearAudioMap()
		{
			audioMap.clear();
		}
	};
}