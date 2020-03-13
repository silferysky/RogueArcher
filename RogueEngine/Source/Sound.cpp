/* Start Header ************************************************************************/
/*!
\file           Sound.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Sound

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Precompiled.h"
#include "Sound.h"
#include "Logger.h"
#include "FMODHelper.h"

namespace Rogue
{
	Sound::Sound() : m_soundOn{ false }, m_result{ FMOD_OK },
		m_system{ 0 }, m_fmodSound{ 0 }, m_channel{ 0 }, m_counter{ 0 }, m_limit { 1 }
	{
	}

	/* Sound Creation */

	void Sound::Create(const char* filename, FMOD::System* fmodSystem, FMOD::DSP* DSPLowPassFilter, FMOD::DSP* DSPHighPassFilter, unsigned limit)
	{
		m_system = fmodSystem;

		if (m_isLooping)
			m_result = m_system->createStream(filename, FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL, 0, &m_fmodSound);
		else
			m_result = m_system->createSound(filename, FMOD_3D | FMOD_3D_LINEARROLLOFF |FMOD_LOOP_OFF | FMOD_CREATESTREAM, 0, &m_fmodSound);

		FmodErrorCheck(m_result);
		m_result = m_fmodSound->setMusicChannelVolume(0, 0);
		FmodErrorCheck(m_result);
		m_result = m_system->playSound(m_fmodSound, 0, true, &m_channel);
		FmodErrorCheck(m_result);

		m_channel->set3DAttributes(0, 0);
		
		m_channel->addDSP(0, DSPLowPassFilter);
		m_channel->addDSP(0, DSPHighPassFilter);

		m_counter = 0;
		m_limit = limit;
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
		//FmodErrorCheck(m_result);
		m_result = m_channel->setVolume(volume);
		//FmodErrorCheck(m_result);
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

	/* void Sound::ReleaseSound()
	{
		m_result = m_fmodSound->release();
		FmodErrorCheck(m_result);
	} */

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
