#include <map>
#include <iostream>
#include "Sound.h"

namespace Rogue
{
	class AudioManager
	{
		std::map<std::string, Sound> audioMap;
	public:
		AudioManager() = default;
		~AudioManager()
		{
			for (auto& audio : audioMap)
			{
				if (audio.second.GetSystem() != NULL)
					audio.second.Release();
			}
		}

		std::map<std::string, Sound> getAudioMap() const
		{
			return audioMap;
		}

		Sound loadSound(std::string audio)
		{
			auto itr = audioMap.find(audio);
			if (itr != audioMap.end())
				return itr->second;
			else
			{
				Sound newSound;
				audioMap.emplace(audio, newSound);

				auto itr1 = audioMap.find(audio);
				return itr1->second;
			}
		}
	};
}