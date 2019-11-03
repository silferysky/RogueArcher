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

		m_SFXstream.Initialize(); // create SFX channel
		m_BGFXstream.Initialize(); // create BGFX channel
		m_BGMstream.Initialize(); // create BGM channel
		m_BGM2stream.Initialize(); // create BGM channel
	}

	void AudioSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Audio System");

		for (auto entity : m_entities)
		{
			if (m_muted)
				break;

			auto& aEmitter = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity);
			auto& sound = aEmitter.getSound();
			Vec2 transformPos{};
			
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
				transformPos = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition();

			float distance = 0.0f;
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(m_trackingTarget))
				distance = Vec2SqDistance(transformPos, g_engine.m_coordinator.GetComponent<TransformComponent>(m_trackingTarget).getPosition());
			
			sound.Update();

			sound.SetVolume(1.0f * aEmitter.getAudioScale() - distance * 0.0015f * 0.0015f);
		}

		for (auto& pair : g_engine.m_coordinator.GetAudioManager().getAudioMap())
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
				m_muted = !m_muted;

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

			if (!(sound.CheckPlaying()))
			{
				sound.CreateBGM(aEmitter.getSoundPath().c_str(), 120.0f, &m_BGFXstream);
				sound.Play(0.3f);
			}
		}

		auto& audioManager = g_engine.m_coordinator.GetAudioManager();

		std::string BGM1path = "Resources/Sounds/[Water Ambience]WATER-CAVE_GEN-HDF-25449.ogg";
		std::string BGM2path = "Resources/Sounds/[Cave Ambience]WIND-HOWL_GEN-HDF-25929.ogg";

		std::string shootSFXpath = "Resources/Sounds/[Shoot Projectile]SCI-FI-WHOOSH_GEN-HDF-20864.ogg";
		std::string elaSFXpath = "Resources/Sounds/[Ela Appear]SCI-FI-WHOOSH_GEN-HDF-20870.ogg";

		auto& BGM1 = audioManager.loadSound(BGM1path);
		auto& BGM2 = audioManager.loadSound(BGM2path);

		auto& shootSFX = audioManager.loadSound(shootSFXpath);
		auto& elaSFX = audioManager.loadSound(elaSFXpath);

		BGM1.CreateBGM(BGM1path.c_str(), 120.0f, &m_BGMstream);
		BGM1.Play(0.5f);

		BGM2.CreateBGM(BGM2path.c_str(), 120.0f, &m_BGM2stream);
		BGM2.Play(0.05f);

		shootSFX.Create(shootSFXpath.c_str(), 1.0f, &m_SFXstream);
		elaSFX.Create(elaSFXpath.c_str(), 2.0f, &m_SFXstream);
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

		for (auto& pair : g_engine.m_coordinator.GetAudioManager().getAudioMap())
		{
			auto& sound = pair.second;

			sound.Pause(true);
			if (sound.GetSystem() != NULL)
				sound.Release();
		}
	}
}