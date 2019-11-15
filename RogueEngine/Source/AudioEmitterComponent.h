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

		float m_audioScale = 1.0f;
	public:
		AudioEmitterComponent() = default;
		~AudioEmitterComponent() = default;

		void setSoundPath(std::string_view soundPath);
		std::string getSoundPath() const;

		void setSound(std::string_view soundPath);
		Sound& getSound();

		void setAudioScale(const float& audioScale);
		float& getAudioScale();

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void Destroy();

		void DisplayOnInspector();
	};
}