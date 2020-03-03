/* Start Header ************************************************************************/
/*!
\file           AudioSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AudioSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Sound.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "Vector2D.h"

namespace Rogue
{
	class Timer;

	class AudioSystem : public System, public EventListener
	{
		FMOD_VECTOR listenerFront;
		FMOD_VECTOR listenerUp;
		bool m_muted;
		Entity m_trackingTarget;
	public:
		AudioSystem();
		~AudioSystem() = default;

		void ToggleMute();
		void TrueInit();
		void InitSounds();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;

		void ShutdownSounds();
	};
}