/* Start Header ************************************************************************/
/*!
\file           AudioEmitterComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AudioEmitterComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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