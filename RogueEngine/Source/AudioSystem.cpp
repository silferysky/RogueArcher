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

		// Set system signature.
		g_engine.m_coordinator.SetSystemSignature<AudioSystem>(signature);

		m_BGMstream.Initialize(); // create BGM channel

		/* Load up BGMs */
		m_music.CreateBGM("Resources/Sounds/hey ya.wav", 1, 112.0f, &m_BGMstream);
		//m_music.Play(0.3f);
	}

	void AudioSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Audio System");

		m_music.Update();

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
		if (m_music.GetSystem() != NULL)
			m_music.Release();

		m_BGMstream.Release();
	}

	void AudioSystem::ToggleMute()
	{
		m_muted = !m_muted;

		/* Mute currently playing BGM */
		if (m_muted)
			FMOD_Channel_SetVolume(m_music.GetChannel(), 0.0f);
		/* Unmute currently playing BGM */
		else
			FMOD_Channel_SetVolume(m_music.GetChannel(), 0.3f);
	}
}