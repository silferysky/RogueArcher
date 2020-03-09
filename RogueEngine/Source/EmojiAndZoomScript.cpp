#include "Precompiled.h"
#include "EmojiAndZoomScript.h"

namespace Rogue
{
	EmojiAndZoomScript::EmojiAndZoomScript(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: TriggerZoom(entity, logicComponent, statsComponent, statsComponent.GetzoomValue(), statsComponent.GetZoomDuration()),
		m_activated{ false },
		m_delayBetweenEmojis{ 1.0f },
		m_timer{ 0.0f }
	{
		m_delayBetweenEmojis = m_statsComponent->GetEmojiDelay();

		for (auto textureStr : m_statsComponent->GetEmojiTextures())
		{
			m_emojiTextures.push(textureStr);
		}
	}

	void EmojiAndZoomScript::AIIdleUpdate()
	{
		TriggerZoom::AIIdleUpdate(); 
		
		//This part is copied from EmojiScript
		if (!m_activated || m_emojiTextures.size() == 0)
			return;

		m_timer -= g_deltaTime * g_engine.GetTimeScale();
		//std::cout << "Timer: " << m_timer << std::endl;

		if (m_timer < 0.0f)
		{
			std::cout << "Swapping Sprites" << std::endl;
			if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
			{
				std::cout << "Init Sprite " << sprite->get().getTexturePath();
				std::ostringstream oss;
				oss << "Resources\\Assets\\" << m_emojiTextures.front();
				sprite->get().setTexturePath(oss.str());
				m_emojiTextures.pop();
				std::cout << "End Sprite " << sprite->get().getTexturePath() << std::endl;
			}

			m_timer = m_delayBetweenEmojis;
		}
	}
	
	void EmojiAndZoomScript::OnTriggerEnter(Entity otherEnt)
	{
		TriggerZoom::OnTriggerEnter(otherEnt);

		//This aprt is copied from EmojiScript
		if (otherEnt != PLAYER_STATUS.GetPlayerEntity())
			return;

		m_activated = true;
		m_timer = 0.0f;			//Set to 0.0f so 1st image will instantly show
	}
}