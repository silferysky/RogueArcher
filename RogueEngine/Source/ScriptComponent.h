/* Start Header ************************************************************************/
/*!
\file           ScriptComponent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BaseAi

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "ILogic.h"
#include "LogicComponent.h"
#include "StatsComponent.h"
#include "Vector2D.h"
#include "Types.h"
#include "PlayerStatusManager.h"
#include "GameEvent.h"

//Default values
#define DEF_PATROL_RANGE 10
#define DEF_DETECT_RANGE 200

#define DT_SPEED_MODIFIER 10000 * g_deltaTime * g_engine.GetTimeScale()
#define DT_TRANSFORM_MODIFIER 100 * g_deltaTime * g_engine.GetTimeScale()

namespace Rogue
{
	class ScriptComponent
		: public ILogic
	{
	public:

		ScriptComponent(Entity entValue, LogicComponent& logicComp, StatsComponent& statsComp);
		virtual ~ScriptComponent() = default;

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
		// Might template this in future...
		virtual void HandleCollision(EntCollisionOrTrigger<BoxCollider2DComponent, BoxCollider2DComponent>& ev);
		//virtual void HandleCollision(EntCollisionOrTrigger<BoxCollider2DComponent, CircleCollider2DComponent>& ev);
		//virtual void HandleCollision(EntCollisionOrTrigger<CircleCollider2DComponent, CircleCollider2DComponent>& ev);

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