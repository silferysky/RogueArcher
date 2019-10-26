#pragma once
#include "ILogic.h"
#include "LogicComponent.h"
#include <memory>
#include <vector>
#include <queue>
#include "Vector2D.h"

namespace Rogue
{
	class BaseAI
		: public ILogic
	{
	public:

		BaseAI(LogicComponent& logicComp);
		virtual ~BaseAI() = default;

		//From ILogic (Component Related)
		virtual void logicInit() override;
		virtual void logicUpdate() override;

		//AI behavior checker to update
		virtual void AIDetect();
		virtual void AIActiveStateUpdate();

		//All of the AI types to update
		virtual void AIChaseUpdate();
		virtual void AIPatrolUpdate();
		virtual void AIIdleUpdate();

		//Getter/Setter
		std::shared_ptr<LogicComponent> getLogicComponent();
		void setLogicComponent(LogicComponent& logicComp);

		void AddWaypoint(Vec2 newPoint);
		void ClearWaypoints();
		std::vector<Vec2> GetWaypoints();

		void AddNextPoint(Vec2 newPoint);
		Vec2 GetNextPoint();
		void PopNextPoint();
		bool NextPointEmpty();

	private:

		std::shared_ptr<LogicComponent> m_logicComponent;
		std::vector<Vec2> m_waypoints;
		std::queue<Vec2> m_nextPoint;
	};
}