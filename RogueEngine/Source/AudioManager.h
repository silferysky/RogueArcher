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

namespace Rogue
{
	class AudioManager
	{
		std::map<std::string, Sound> audioMap;
		std::string directory = "Resources/Sounds/";
	public:
		static AudioManager& instance()
		{
			static AudioManager instance;
			return instance;
		}

		AudioManager() = default;
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
				return itr->second;
			else
			{
				Sound newSound;
				newSound.m_volume = volume;
				newSound.m_isLooping = looping;
				newSound.Create(audio.c_str());
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