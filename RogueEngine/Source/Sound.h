/* Start Header ************************************************************************/
/*!
\file           Sound.h
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
#include "Vector2D.h"

// FMOD Headers
#include "fmod.hpp"
#include "fmod_errors.h"

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
		//FMOD::DSP* m_DSPLowPassFilter; 
		//FMOD::DSP* m_DSPHighPassFilter;
		//FMOD::Reverb3D* m_reverb;

		/* Sound Statistics */
		bool m_isLooping = false;
		float m_volume = 0.0f;
		unsigned m_counter = 0;
		unsigned m_limit = 1;

		Sound();

		/* FMOD sound/channel/system creation */
		void Create(const char* filename, FMOD::System* fmodSystem, FMOD::DSP* DSPLowPassFilter, FMOD::DSP* DSPHighPassFilter, unsigned limit = 1);
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