/* Start Header ************************************************************************/
/*!
\file           CoraleElementBubble.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CoraleElementBubble

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
	class CoralElementBubble
		: public ScriptComponent
	{
	public:
		CoralElementBubble(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void LogicInit() override;
		virtual void AIActiveStateUpdate() override;
		virtual void AIPatrolUpdate() override;
		virtual void OnTriggerEnter(Entity other) override;
		
	protected:
		std::vector<Vec2> m_waypoints;
		size_t m_currentPointIndex;
		float m_patrolDelay;
		float m_delay;
	};
}