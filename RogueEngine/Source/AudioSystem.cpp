#include "Precompiled.h"
#include "AudioSystem.h"
#include "REEngine.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"

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
		signature.set(g_engine.m_coordinator.GetComponentType<AudioEmitterComponent>());

		// Set system signature.
		g_engine.m_coordinator.SetSystemSignature<AudioSystem>(signature);
	}

	void AudioSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Audio System");

		for (auto entity : m_entities)
		{
			if (m_muted)
				break;

			auto& aEmitter = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity);

			if (!aEmitter.getIsScaling())
				continue;

			auto sound = aEmitter.getSound();
			Vec2 transformPos{};
			
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
				transformPos = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).GetPosition();

			float distance = 0.0f;
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(m_trackingTarget))
				distance = Vec2SqDistance(transformPos, g_engine.m_coordinator.GetComponent<TransformComponent>(m_trackingTarget).GetPosition());
			
			sound.Update();

			sound.SetVolume(1.0f * aEmitter.getAudioScale() - distance * 0.0015f * 0.0015f);
		}

		for (auto& pair : AudioManager::instance().getAudioMap())
		{
			pair.second.Update();
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
			g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).DestroySound();
		}
	}

	void AudioSystem::ToggleMute()
	{
		m_muted = !m_muted;

		//TODO:

	}

	void AudioSystem::TrueInit()
	{
		for (auto& objectIterator : g_engine.m_coordinator.GetActiveObjects())
		{
			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(objectIterator))
			{
				m_trackingTarget = objectIterator;

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

			aEmitter.CreateSound();
		}
	}

	void AudioSystem::ShutdownSounds()
	{
		for (auto entity : m_entities)
		{
			g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).DestroySound();
		}
	}
}