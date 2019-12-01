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

		bool m_isScaling;
		float m_maxDistance = 1.0f;
	public:
		AudioEmitterComponent() = default;
		~AudioEmitterComponent() = default;

		void setSoundPath(std::string_view soundPath);
		std::string getSoundPath() const;

		void CreateSound();
		Sound getSound();

		void setIsLooping(bool isLooping);
		bool& getIsLooping();

		void setIsScaling(bool isScaling);
		bool& getIsScaling();

		void setMaxDistance(const float audioScale);
		float& getMaxDistance();

		void setVolume(const float volume);
		float& getVolume();

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DestroySound();

		void DisplayOnInspector();
	};
}