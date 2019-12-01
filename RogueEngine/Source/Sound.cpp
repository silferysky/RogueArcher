#pragma once
#include "Precompiled.h"
#include "Sound.h"
#include "Logger.h"
#include "Main.h"
#include "FMODHelper.h"

namespace Rogue
{
	Sound::Sound() : m_soundOn{ false }, m_result{ FMOD_OK },
		m_system{ 0 }, m_fmodSound{ 0 }, m_channel{ 0 }
	{
	}

	/* Sound Creation */

	void Sound::Create(const char* filename)
	{
		m_result = FMOD::System_Create(&m_system);
		FmodErrorCheck(m_result);
		m_result = m_system->init(64, FMOD_INIT_NORMAL, 0);
		FmodErrorCheck(m_result);

		if (m_isLooping)
			m_result = m_system->createStream(filename, FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL, 0, &m_fmodSound);
		else
			m_result = m_system->createSound(filename, FMOD_3D | FMOD_3D_LINEARROLLOFF |FMOD_LOOP_OFF | FMOD_CREATESTREAM, 0, &m_fmodSound);

		FmodErrorCheck(m_result);
		m_result = m_fmodSound->setMusicChannelVolume(0, 0);
		FmodErrorCheck(m_result);
		m_result = m_system->playSound(m_fmodSound, 0, true, &m_channel);
		FmodErrorCheck(m_result);

		m_system->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &m_DSPLowPassFilter);
		m_channel->addDSP(0, m_DSPLowPassFilter);
		m_DSPLowPassFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 1200.0f);

		m_system->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &m_DSPHighPassFilter);
		m_channel->addDSP(0, m_DSPHighPassFilter);
		m_DSPLowPassFilter->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, 1200.0f);

		FMOD_REVERB_PROPERTIES reverbProperties = FMOD_PRESET_CAVE;
		m_system->createReverb3D(&m_reverb);
		m_reverb->setProperties(&reverbProperties);
		m_reverb->set3DAttributes(0, 0.0f, 10000.0f);
	}

	void Sound::Set3DLocation(Vec2 pos)
	{
		FMOD_VECTOR position{ pos.x, pos.y, 0 };
		m_channel->set3DAttributes(&position, 0);
	}

	void Sound::Set3DMaxDistance(float max)
	{
		m_channel->set3DMinMaxDistance(0.0f, max);
	}

	/* General Audio Functions */

	void Sound::Play(float volume)
	{
		m_soundOn = true;
		m_result = m_system->playSound(m_fmodSound, 0, false, &m_channel);
		FmodErrorCheck(m_result);
		m_result = m_channel->setVolume(volume);
		FmodErrorCheck(m_result);
	}

	void Sound::Update()
	{
		m_result = m_system->update();
		FmodErrorCheck(m_result);
	}

	void Sound::Pause(FMOD_BOOL pause)
	{
		m_channel->setPaused(pause);
		FmodErrorCheck(m_result);
	}

	void Sound::Unload()
	{
		m_soundOn = false;
		m_fmodSound->release();
		FmodErrorCheck(m_result);
	}

	void Sound::Release()
	{
		m_result = m_fmodSound->release();
		FmodErrorCheck(m_result);

		m_result = m_system->release();
		FmodErrorCheck(m_result);
	}

	/* Getters/Setters */
	FMOD::System* Sound::GetSystem()
	{
		return m_system;
	}

	FMOD::Sound* Sound::GetFmodSound()
	{
		return m_fmodSound;
	}

	FMOD::Channel* Sound::GetChannel()
	{
		return m_channel;
	}

	float Sound::GetVolume()
	{
		float volume;
		m_channel->getVolume(&volume);
		return volume;
	}

	void Sound::SetVolume(float volume)
	{
		if (volume < 0)
			m_channel->setVolume(0.0f);
		else
			m_channel->setVolume(volume);
	}

	bool Sound::CheckPlaying()
	{
		m_channel->isPlaying(&m_soundOn);
		FmodErrorCheck(m_result);
		return m_soundOn;
	}
}
