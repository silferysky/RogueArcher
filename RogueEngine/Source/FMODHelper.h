/* Start Header ************************************************************************/
/*!
\file           FMODHelper.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for FMODHelper

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
