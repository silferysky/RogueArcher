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
	enum class SystemID
	{
		id_MEMORYMANAGER = 0,
		id_INPUTMANAGER,
		id_WINDOWSYSTEM,
		id_GRAPHICSSYSTEM,
		id_ANIMATIONSYSTEM,
		id_MASKINGSYSTEM,
		id_FADESYSTEM,
		id_DEBUGDRAWSYSTEM,
		id_CAMERASYSTEM,
		id_FONTSYSTEM,
		id_AUDIOSYSTEM,
		id_PHYSICSSYSTEM,
		id_COLLISIONTAGSYSTEM,
		id_COLLISIONSYSTEM,
		id_BOXCOLLISIONSYSTEM,
		id_CIRCLECOLLISIONSYSTEM,
		id_LOGICSYSTEM,
		id_PLAYERCONTROLLERSYSTEM,
		id_MENUCONTROLLERSYSTEM,
		id_TESTSYSTEM,
		id_EDITOR,
		id_CURSORSYSTEM,
		id_PICKINGSYSTEM,
		id_PARTICLESYSTEM,
		id_PARTICLEEMITTERSYSTEM,
		id_UISYSTEM,
		id_PARENTCHILDSYSTEM,

		id_LASTSYS
	};
}