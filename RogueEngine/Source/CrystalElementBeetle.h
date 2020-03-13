/* Start Header ************************************************************************/
/*!
\file           PatrolAI.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PatrolAI

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
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
	class CrystalElementBeetle
		: public ScriptComponent
	{
	public:
		CrystalElementBeetle(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void LogicInit() override;
		virtual void AIActiveStateUpdate() override;
		virtual void AIPatrolUpdate() override;
	protected:

		void SwapCollisionType();
		void SetCollisionColor();

		std::vector<Vec2> m_waypoints;
		size_t m_currentPointIndex;
		float m_patrolDelay;
		float m_delay;
		bool m_swapping;
	};
}