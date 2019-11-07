#pragma once
#include "ILogic.h"
#include "LogicComponent.h"
#include <memory>
#include <vector>
#include <queue>
#include "Vector2D.h"
#include "Types.h"

#define DEF_PATROL_RANGE 10
#define DEF_DETECT_RANGE 200
#define DEF_SPEED 10000 * g_deltaTime
#define DEF_TRANSFORM_SPEED 100 * g_deltaTime

namespace Rogue
{
	class BaseAI
		: public ILogic
	{
	public:

		BaseAI(Entity entValue, LogicComponent& logicComp);
		virtual ~BaseAI() = default;

		//From ILogic (Component Related)
		virtual void LogicInit() override;
		virtual void LogicUpdate() override;

		//AI behavior checker to update
		virtual void AIDetect();
		virtual void AIActiveStateUpdate();

		//All of the AI types to update
		virtual void AIChaseUpdate();
		virtual void AIPatrolUpdate();
		virtual void AIIdleUpdate();

		//Getter/Setter
		std::shared_ptr<LogicComponent> GetLogicComponent();
		void SetLogicComponent(LogicComponent& logicComp);

		void AddNextPoint(Vec2 newPoint);
		Vec2 GetNextPoint();
		void PopNextPoint();
		bool NextPointEmpty();

	protected:

		Entity m_entity;
		std::shared_ptr<LogicComponent> m_logicComponent;
		std::queue<Vec2> m_nextPoint;
	};
}