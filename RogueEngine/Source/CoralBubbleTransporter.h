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
		bool m_isMoving;
	};
}

