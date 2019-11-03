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

		m_BGMstream.Initialize(); // create BGM channel
	}

	void AudioSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Audio System");

		for (auto entity : m_entities)
		{
			auto& sound = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound();
			auto transformPos = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition();

			float distance = Vec2Distance(transformPos, g_engine.m_coordinator.GetComponent<TransformComponent>(m_trackingTarget).getPosition());
			
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

		m_BGMstream.Release();
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
				sound.CreateBGM(aEmitter.getSoundPath().c_str(), 1, 112.0f, &m_BGMstream);
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