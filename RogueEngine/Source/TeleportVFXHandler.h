/* Start Header ************************************************************************/
/*!
\file           TeleportVFXHandler.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for TeleportVFXHandler

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class TeleportVFXHandler
		: public  ScriptComponent
	{
	public:
		TeleportVFXHandler(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

	private:
		std::vector<Entity> m_teleportVFXArray;
		std::vector<float> m_timers;
	};
}