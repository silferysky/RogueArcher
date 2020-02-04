#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class TeleCharge1
		: public ScriptComponent
	{
	public:
		TeleCharge1(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge = 0.0f);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		void SetTeleCharge(float max);
		float GetTeleCharge() const;
	private:
		float m_teleCharge;
	};
}