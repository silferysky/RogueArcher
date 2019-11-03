#include "AudioSystem.h"
#include "REEngine.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"
#include <cmath>


// fast log2 function
float log2_fast(double d) 
{
	int result;
	void(std::frexp(d, &result));
	return result - 1.0f;
}

namespace Rogue
{
	//-------------------------------------------------------//
	//              PUBLIC MEMBER FUNCTIONS					 //
	//-------------------------------------------------------//

	AudioSystem::AudioSystem()
		: System(SystemID::id_AUDIOSYSTEM), m_muted{ false }
	{}

	void AudioSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_AUDIOSYSTEM, AudioSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<AudioEmitterComponent>());

		// Set system signature.
		g_engine.m_coordinator.SetSystemSignature<AudioSystem>(signature);

		m_BGFXstream.Initialize(); // create BGFX channel
		m_BGMstream.Initialize(); // create BGM channel

		m_BGM.CreateBGM("Resources/Sounds/[Water Ambience]WATER-CAVE_GEN-HDF-25449.ogg", 1, 112.0f, &m_BGMstream);
		m_BGM.Play(0.5f);
	}

	void AudioSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Audio System");

		for (auto entity : m_entities)
		{
			auto& sound = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound();
			Vec2 transformPos{};
			
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
				transformPos = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition();

			float distance = 0.0f;
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(m_trackingTarget))
				distance = Vec2SqDistance(transformPos, g_engine.m_coordinator.GetComponent<TransformComponent>(m_trackingTarget).getPosition());
			
			sound.Update();

			sound.SetVolume(1.0f - distance * 0.0015f);
		}

		g_engine.m_coordinator.EndTimeSystem("Audio System");
	}

	void AudioSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode();

			if (keycode == KeyPress::KeyM)
				ToggleMute();

			return;
		} //End KeyTriggered
		}
	}

	void AudioSystem::Shutdown()
	{
		for (auto entity : m_entities)
		{
			auto sound = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound();
			if (sound.GetSystem() != NULL)
				sound.Release();
		}

		m_BGFXstream.Release();
	}

	void AudioSystem::ToggleMute()
	{
		m_muted = !m_muted;

		/* Mute currently playing BGM */
		if (m_muted)
		{
			for (auto entity : m_entities)
			{
				auto sound = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound();
				sound.SetVolume(0.0f);
			}
		}
		/* Unmute currently playing BGM */
		else
		{
			for (auto entity : m_entities)
			{
				auto sound = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound();
				sound.SetVolume(0.3f);
			}
		}
	}

	void AudioSystem::TrueInit()
	{
		for (auto& objectIterator : g_engine.m_coordinator.GetActiveObjects())
		{
			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(objectIterator.m_Entity))
			{
				m_trackingTarget = objectIterator.m_Entity;

				return;
			}
		}

		InitSounds();
	}

	void AudioSystem::InitSounds()
	{
		for (auto entity : m_entities)
		{
			auto& aEmitter = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity);
			auto& sound = aEmitter.getSound();

			/* Load up BGMs */
			if (!(sound.m_b_IsPlaying))
			{
				sound.CreateBGM(aEmitter.getSoundPath().c_str(), 1, 112.0f, &m_BGFXstream);
				sound.Play(0.3f);
			}
		}
	}

	void AudioSystem::ShutdownSounds()
	{
		for (auto entity : m_entities)
		{
			auto sound = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound();

			sound.Pause(true);
			if (sound.GetSystem() != NULL)
				sound.Release();
		}
	}
}