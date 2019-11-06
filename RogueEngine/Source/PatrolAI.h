#pragma once
#include "BaseAI.h"

#define PATROL_RANGE 1
#define SPEED 1

namespace Rogue
{
	class PatrolAI
		: public BaseAI
	{
	public:
		PatrolAI(Entity entity, LogicComponent& logicComponent);

		virtual void AIActiveStateUpdate() override;

		virtual void AIPatrolUpdate() override;

		void AddWaypoint(Vec2 newPoint);
		void ClearWaypoints();
		std::vector<Vec2> GetWaypoints();
		
	private:
		std::vector<Vec2> m_waypoints;
		size_t m_currentPointIndex;
	};
}