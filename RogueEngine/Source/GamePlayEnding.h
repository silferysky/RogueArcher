#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class GamePlayEnding : public ScriptComponent
	{
		int m_souls = 0;
		float m_timer = 0.0f;
		bool m_activated = false;
		float m_movement = 15.0f;
		bool m_moveleft;
		bool m_moveright;
		bool m_finalInput;
		bool m_endingAPressed;
		bool m_endingDPressed;
		bool m_trueEnding;
		bool m_finalSpriteSet;
		Entity m_finalSprite;
	public:
		GamePlayEnding(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
		virtual void OnTriggerEnter(Entity other) override;

		bool CrystalComplete;
		bool CoralComplete;
		bool VegetationComplete;
	};
}
