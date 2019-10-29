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
		Stream m_BGMstream;
		Sound m_music;
		bool m_muted;

		Vec2* m_trackingTarget;
	public:
		AudioSystem();
		~AudioSystem() = default;

		void ToggleMute();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}