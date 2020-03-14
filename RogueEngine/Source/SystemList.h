/* Start Header ************************************************************************/
/*!
\file           SystemList.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for SystemList

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

namespace Rogue
{
	// Have to put in ORDER of system registration order in REEngine.h.
	enum class SystemID
	{
		id_INPUTMANAGER = 0,
		id_LOGICSYSTEM,
		id_SOULSYSTEM,
		id_PARTICLESYSTEM,
		id_PARTICLEEMITTERSYSTEM,
		id_CURSORSYSTEM,
		id_PICKINGSYSTEM,
		id_PLAYERCONTROLLERSYSTEM,
		id_MENUCONTROLLERSYSTEM,
		id_PHYSICSSYSTEM,
		id_CIRCLECOLLISIONSYSTEM,
		id_BOXCOLLISIONSYSTEM,
		id_COLLISIONSYSTEM,
		id_GRAPHICSSYSTEM,
		id_FADESYSTEM,
		id_MASKINGSYSTEM,
		id_ANIMATIONSYSTEM,
		id_DEBUGDRAWSYSTEM,
		id_FONTSYSTEM,
		id_CAMERASYSTEM,
		id_AUDIOSYSTEM,
		id_UISYSTEM,
		id_LIGHTINGSYSTEM,
		id_PARENTCHILDSYSTEM,
		id_EDITOR,
		id_TILEMAPSYSTEM,

		// Unused
		id_MEMORYMANAGER,
		id_WINDOWSYSTEM,
		id_TESTSYSTEM,
		id_COLLISIONTAGSYSTEM,

		id_LASTSYS
	};

	// Also remember to add your system manually into Update() or FixedUpdate() in SystemManager::UpdateSystems()
}