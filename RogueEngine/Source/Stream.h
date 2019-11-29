#pragma once
#include "fmod_errors.h"

namespace Rogue
{
	struct Stream
	{
		FMOD_SYSTEM* m_system;	/* the system where the sound will be using */
		FMOD_RESULT	m_result;		/* allows error checking for FMOD functions */
		/* Constructor */
		Stream();
		/* Error-checking*/
		void FmodErrorCheck(FMOD_RESULT result);
		/* FMOD initialisation */
		void Initialize();
		/* Release from system */
		void Release();
	};

}