#pragma once
#include "Precompiled.h"
#include "Sound.h"
#include "Logger.h"
#include "Main.h"

namespace Rogue
{
	Sound::Sound() : m_soundOn{ false }, m_canPlaySound{ true }, currentSound{ 0 }, m_result{ FMOD_OK },
		m_system{ 0 }, m_fmodSound{ 0 }, m_channel{ 0 }
	{
	}

	void Sound::FmodErrorCheck(FMOD_RESULT resultCheck)
	{
		// Throw an error if FMOD finds something wrong
		if (resultCheck != FMOD_OK)
		{
			std::ostringstream ss;
			ss << "FMOD error: " << resultCheck << FMOD_ErrorString(resultCheck);
			RE_INFO(ss.str());
		}
	}

	/* Sound Creation */

	// For SFX
	void Sound::Create(const char* filename, float playTimer)
	{
		m_result = FMOD_System_Create(&m_system);
		FmodErrorCheck(m_result);
		m_result = FMOD_System_Init(m_system, 64, FMOD_INIT_NORMAL, 0);
		FmodErrorCheck(m_result);
		m_f_PlayTimer = playTimer;
		m_result = FMOD_System_CreateSound(m_system, filename, FMOD_LOOP_OFF | FMOD_CREATESTREAM, 0, &m_fmodSound);
		//FMOD_Channel_Set3DAttributes(m_channel,);
		FmodErrorCheck(m_result);
		m_result = FMOD_System_PlaySound(m_system, m_fmodSound, 0, true, &m_channel);
		FmodErrorCheck(m_result);
	}

	// For BGM
	void Sound::CreateBGM(const char* filename, float playTimer, int counterCap)
	{
		m_result = FMOD_System_Create(&m_system);
		FmodErrorCheck(m_result);
		m_result = FMOD_System_Init(m_system, 64, FMOD_INIT_NORMAL, 0);
		FmodErrorCheck(m_result);
		m_c_PlayCap = counterCap;
		m_f_PlayTimer = playTimer;
		m_result = FMOD_System_CreateStream(m_system, filename, FMOD_LOOP_NORMAL, 0, &m_fmodSound);
		FmodErrorCheck(m_result);
		m_result = FMOD_Sound_SetMusicChannelVolume(m_fmodSound, 0, 0);
		FmodErrorCheck(m_result);
		m_result = FMOD_System_PlaySound(m_system, m_fmodSound, 0, true, &m_channel);
		FmodErrorCheck(m_result);
	}

	/* General Audio Functions */

	void Sound::Play(float volume)
	{
		if (m_c_PlayCounter < m_c_PlayCap)
		{
			m_soundOn = true;
			m_result = FMOD_System_PlaySound(m_system, m_fmodSound, 0, false, &m_channel);
			m_b_IsPlaying = true;
			FmodErrorCheck(m_result);
			m_result = FMOD_Channel_SetVolume(m_channel, volume);
			FmodErrorCheck(m_result);
			++m_c_PlayCounter;
			m_f_Timer = 0;
		}
	}

	void Sound::Update()
	{
		m_result = FMOD_System_Update(m_system);
		FmodErrorCheck(m_result);

		if (m_f_Timer > m_f_PlayTimer)
		{
			m_f_Timer = 0.0f;
			m_c_PlayCounter = 0;
		}
		if (m_b_IsPlaying)
		{
			m_f_Timer += g_deltaTime;
		}
	}

	void Sound::Pause(FMOD_BOOL pause)
	{
		FMOD_Channel_SetPaused(m_channel, pause);
		m_b_IsPlaying = false;
		FmodErrorCheck(m_result);
	}

	void Sound::Unload()
	{
		currentSound = 0;
		m_soundOn = false;
		FMOD_Sound_Release(m_fmodSound);
		FmodErrorCheck(m_result);
	}

	void Sound::Release()
	{
		FMOD_Sound_Release(m_fmodSound);
		FmodErrorCheck(m_result);

		m_result = FMOD_System_Release(m_system);
		FmodErrorCheck(m_result);
	}

	/* Getters/Setters */
	FMOD_SYSTEM* Sound::GetSystem()
	{
		return m_system;
	}

	FMOD_SOUND* Sound::GetFmodSound()
	{
		return m_fmodSound;
	}

	FMOD_CHANNEL* Sound::GetChannel()
	{
		return m_channel;
	}

	float Sound::GetVolume()
	{
		float volume;
		FMOD_Channel_GetVolume(m_channel, &volume);
		return volume;
	}

	void Sound::SetVolume(float volume)
	{
		if (volume < 0)
			FMOD_Channel_SetVolume(m_channel, 0.0f);
		else
			FMOD_Channel_SetVolume(m_channel, volume);
	}

	void Sound::ResetSoundCounter()
	{
		m_f_Timer = 0;
		m_c_PlayCounter = 0;
	}

	bool Sound::CheckPlaying()
	{
		FMOD_Channel_IsPlaying(m_channel, &m_soundOn);
		FmodErrorCheck(m_result);
		return m_soundOn;
	}
}
