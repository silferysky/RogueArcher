#pragma once
#include "ScriptComponent.h"
#include <queue>

namespace Rogue
{
	class EmojiScript
		: public ScriptComponent
	{
	public:
		EmojiScript(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		void LogicInit() override;

		void AIIdleUpdate() override;

		void OnTriggerEnter(Entity otherEnt) override;

	private:
		bool m_activated;
		bool m_finished;
		std::queue<std::string> m_emojiTextures;
		float m_delayBetweenEmojis;
		float m_timer;
	};
}