#pragma once
#include "Precompiled.h"
#include "Logger.h"

namespace Rogue
{
	static void FmodErrorCheck(FMOD_RESULT resultCheck)
	{
		// Throw an error if FMOD finds something wrong
		if (resultCheck != FMOD_OK)
		{
			std::ostringstream ss;
			ss << "FMOD error: " << resultCheck << FMOD_ErrorString(resultCheck);
			RE_INFO(ss.str());
		}
	}

	static void InitSystem(FMOD::System* system)
	{
		FMOD_RESULT m_result;
		m_result = FMOD::System_Create(&system);
		FmodErrorCheck(m_result);
		m_result = system->init(64, FMOD_INIT_NORMAL, 0);
		FmodErrorCheck(m_result);
	}
}
