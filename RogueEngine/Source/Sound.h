#pragma once
#include "Stream.h"

namespace Rogue
{
	class Sound
	{
		FMOD_BOOL m_soundOn;		/* is sound on? */
		FMOD_BOOL m_canPlaySound;	/* is it possible to play sound? */
		char* currentSound;	/* currently played sound */

		/* FMOD-specific */
		FMOD_SYSTEM* m_system;	/* the system where the sound will be using */
		FMOD_RESULT	m_result;		/* allows error checking for FMOD functions */
		FMOD_SOUND* m_fmodSound;	/* holding the actual sound */
		FMOD_CHANNEL* m_channel;	/* the channel where the sound will be playing from */

		/* Sound Statistics */
		float m_f_Timer = 0.0f;
		float m_f_PlayTimer = 0.0f;
		bool m_b_IsPlaying = false;
		int m_c_PlayCounter = 0;
		int m_c_PlayCap = 10000;
	public:

		/* Constructor */
		Sound();
		/* Error-checking*/
		void FmodErrorCheck(FMOD_RESULT result);

		/* FMOD sound/channel/system creation */
		void CreateBGM(const char* filename, float playTimer, Stream* audioPtr, int counterCap = 1);
		void Create(const char* filename, float playTimer, Stream* audioPtr);

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
		void ResetSoundCounter();
		bool CheckPlaying();
	};
}