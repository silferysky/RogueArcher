#pragma once
#include "Precompiled.h"
#include "ILogic.h"
#include "LogicComponent.h"
#include "StatsComponent.h"
#include "Vector2D.h"
#include "Types.h"
#include "GameEvent.h"

//Default values
#define DEF_PATROL_RANGE 10
#define DEF_DETECT_RANGE 200

#define DT_SPEED_MODIFIER 10000 * g_deltaTime
#define DT_TRANSFORM_MODIFIER 100 * g_deltaTime

namespace Rogue
{
	class BaseAI
		: public ILogic
	{
	public:

		BaseAI(Entity entValue, LogicComponent& logicComp, StatsComponent& statsComp);
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

		//Collision handlers
		virtual void HandleCollision(EntCollisionOrTrigger* ev);

		virtual void OnCollisionEnter(Entity otherEnt);
		virtual void OnCollisionStay(Entity otherEnt);
		virtual void OnCollisionExit(Entity otherEnt);
		virtual void OnTriggerEnter(Entity otherEnt);
		virtual void OnTriggerStay(Entity otherEnt);
		virtual void OnTriggerExit(Entity otherEnt);

		//Getter/Setter
		std::shared_ptr<LogicComponent> GetLogicComponent();
		void SetLogicComponent(LogicComponent& logicComp);
		std::shared_ptr<StatsComponent> GetStatsComponent();
		void SetStatsComponent(StatsComponent& statsComp);

		void AddNextPoint(Vec2 newPoint);
		Vec2 GetNextPoint();
		void PopNextPoint();
		bool NextPointEmpty();

	protected:

		Entity m_entity;
		std::shared_ptr<LogicComponent> m_logicComponent;
		std::shared_ptr<StatsComponent> m_statsComponent;
		std::queue<Vec2> m_nextPoint;
	};
}