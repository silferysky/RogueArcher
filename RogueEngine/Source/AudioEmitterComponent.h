#pragma once
#include "BaseComponent.h"
#include "Sound.h"

namespace Rogue
{
	class AudioEmitterComponent
		: public BaseComponent
	{
		std::string m_soundPath;
		Sound m_sound;
	public:
		AudioEmitterComponent() = default;
		~AudioEmitterComponent() = default;

		void setSoundPath(const std::string& soundPath);
		std::string getSoundPath() const;

		void setSound(const std::string& soundPath);
		Sound& getSound();

		std::string Serialize();
		void Deserialize(std::string toDeserialize);

		void Destroy();
	};
}