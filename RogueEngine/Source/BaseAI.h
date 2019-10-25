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
		void logicInit() override;
		void logicUpdate() override;

		//From ILogic (Logic related)
		void AIDetect() override;
		void AIActiveStateUpdate() override;

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