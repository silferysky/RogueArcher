#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class TeleCharge3
		: public ScriptComponent
	{
	public:
		TeleCharge3(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge = 0.0f);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		void SetTeleCharge(float max);
		float GetTeleCharge() const;

	private:
		float m_teleCharge;
		float m_timer;
	};
}
