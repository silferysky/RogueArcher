#include "Precompiled.h"
#include "Stream.h"
#include "Logger.h"

namespace Rogue
{
	/* Constructor */
	Stream::Stream() : m_system{ 0 }, m_result{ FMOD_OK }
	{
	}

	/* Error check */
	void Stream::FmodErrorCheck(FMOD_RESULT resultCheck)
	{
		/* Throw an error if FMOD finds something wrong */
		if (resultCheck != FMOD_OK)
			RE_INFO("FMOD error:", resultCheck, FMOD_ErrorString(resultCheck));
	}

	/* General System Functions */
	void Stream::Initialize()
	{
		FMOD_System_Create(&m_system);
		FmodErrorCheck(m_result);
		FMOD_System_Init(m_system, 64, FMOD_INIT_NORMAL, 0);
		FmodErrorCheck(m_result);
	}

	void Stream::Release()
	{
		FMOD_System_Release(m_system);
		FmodErrorCheck(m_result);
	}
}
