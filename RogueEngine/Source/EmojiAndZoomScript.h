#pragma once
#include "TriggerZoom.h"

namespace Rogue
{
	class EmojiAndZoomScript
		: public TriggerZoom
	{
	public:
		EmojiAndZoomScript(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		void AIIdleUpdate() override;

		void OnTriggerEnter(Entity otherEnt) override;

	private:
		bool m_activated;
		std::queue<std::string> m_emojiTextures;
		float m_delayBetweenEmojis;
		float m_timer;
	};
}