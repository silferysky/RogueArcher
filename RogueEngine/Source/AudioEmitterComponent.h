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

		bool m_isLooping;
		bool m_isScaling;
		float m_audioScale = 1.0f;
		float m_volume = 0.3f;
	public:
		AudioEmitterComponent() = default;
		~AudioEmitterComponent() = default;

		void setSoundPath(std::string_view soundPath);
		std::string getSoundPath() const;

		void CreateSound();
		Sound& getSound();

		void setIsLooping(bool isLooping);
		bool& getIsLooping();

		void setIsScaling(bool isScaling);
		bool& getIsScaling();

		void setAudioScale(const float audioScale);
		float& getAudioScale();

		void setVolume(const float volume);
		float& getVolume();

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DestroySound();

		void DisplayOnInspector();
	};
}