#pragma once
#include "Stream.h"

namespace Rogue
{
	struct Sound
	{
		FMOD_BOOL m_soundOn;		/* is sound on? */

		// FMOD-specific
		FMOD_SYSTEM* m_system;	/* the system where the sound will be using */
		FMOD_RESULT	m_result;		/* allows error checking for FMOD functions */
		FMOD_SOUND* m_fmodSound;	/* holding the actual sound */
		FMOD_CHANNEL* m_channel;	/* the channel where the sound will be playing from */

		/* Sound Statistics */
		bool m_isLooping = false;
		float m_volume = 0.0f;

		Sound();
		/* Error-checking*/
		void FmodErrorCheck(FMOD_RESULT result);

		/* FMOD sound/channel/system creation */
		void Create(const char* filename);

		/* General Audio Functions */
		/* Play the sound */
		void Play(float volume = 0.33f);
		/* Update functions */
		void Update();
		/* Pauses the sound */
		void Pause(FMOD_BOOL pause);
		/* Unload sound */
		void Unload();
		/* Unload from memory */
		void Release();

		/* Getter/Setter */
		FMOD_SYSTEM* GetSystem();
		FMOD_SOUND* GetFmodSound();
		FMOD_CHANNEL* GetChannel();

		float GetVolume();
		void SetVolume(float volume);
		bool CheckPlaying();
	};
}