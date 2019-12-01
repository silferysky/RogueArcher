#pragma once
#include "Vector2D.h"

namespace Rogue
{
	struct Sound
	{
		bool m_soundOn;		/* is sound on? */

		// FMOD-specific
		FMOD::System* m_system;	/* the system where the sound will be using */
		FMOD_RESULT	m_result;		/* allows error checking for FMOD functions */
		FMOD::Sound* m_fmodSound;	/* holding the actual sound */
		FMOD::Channel* m_channel;	/* the channel where the sound will be playing from */
		FMOD::DSP* m_DSPLowPassFilter; 
		FMOD::DSP* m_DSPHighPassFilter;
		FMOD::Reverb3D* m_reverb;

		/* Sound Statistics */
		bool m_isLooping = false;
		float m_volume = 0.0f;

		Sound();

		/* FMOD sound/channel/system creation */
		void Create(const char* filename);
		void Set3DLocation(Vec2 pos);
		void Set3DMaxDistance(float max);

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
		FMOD::System* GetSystem();
		FMOD::Sound* GetFmodSound();
		FMOD::Channel* GetChannel();

		float GetVolume();
		void SetVolume(float volume);
		bool CheckPlaying();
	};
}