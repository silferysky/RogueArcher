/* Start Header ************************************************************************/
/*!
\file           CoralBubbleTransporter.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for CoralBubbleTransporter

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
	class CoralBubbleTransporter
		: public ScriptComponent
	{
	public:
		CoralBubbleTransporter(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		virtual void OnTriggerEnter(Entity other) override;

		virtual void LogicInit() override;
		virtual void AIPatrolUpdate() override;
	private:
		std::vector<Vec2> m_waypoints;
		size_t m_currentPointIndex;
		float m_patrolDelay;
		float m_delay;
		bool m_isPopping;
	};
}

