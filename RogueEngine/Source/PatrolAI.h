#pragma once
#include "BaseAI.h"

namespace Rogue
{
	class PatrolAI
		: public BaseAI
	{
		bool m_isPatrolling = true;

	public:
		PatrolAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void LogicInit() override;
		virtual void AIActiveStateUpdate() override;
		virtual void AIPatrolUpdate() override;

		void AddWaypoint(Vec2 newPoint);
		void ClearWaypoints();
		std::vector<Vec2> GetWaypoints();

		void SetIsPatrolling(bool patrol);
		
	private:
		std::vector<Vec2> m_waypoints;
		size_t m_currentPointIndex;
	};
}