#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class TeleportVFXHandler
		: public  ScriptComponent
	{
	public:
		TeleportVFXHandler(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

	private:
		std::vector<Entity> m_teleportVFXArray;
		float m_timer;
		float m_delay;
	};
}