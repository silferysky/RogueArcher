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

		void setID(const std::string& id);
		std::string getID() const;

		Sound* getSound();
		void Destroy();

		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}