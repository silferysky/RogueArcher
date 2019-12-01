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
		void Receive(Event* ev) override;

		void ShutdownSounds();
	};
}