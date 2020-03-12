#include "Precompiled.h"
#include "EmojiScript.h"

namespace Rogue
{
	EmojiScript::EmojiScript(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent),
		m_activated{ false },
		m_finished{ false },
		m_delayBetweenEmojis { statsComponent.GetEmojiDelay() },
		m_timer {0.0f}
	{
		LogicInit();
	}

	void EmojiScript::LogicInit()
	{
		//m_delayBetweenEmojis = m_statsComponent->GetEmojiDelay();

		for (auto textureStr : m_statsComponent->GetEmojiTextures())
		{
			m_emojiTextures.push(textureStr);
		}
	}

	void EmojiScript::AIIdleUpdate()
	{
		//This part is copied from EmojiScript
		if (!m_activated || m_finished)
			return;

		m_timer -= g_deltaTime;
		//std::cout << "Emoji Timer: " << m_timer << std::endl;

		if (m_timer < 0.0f)
		{
			//If no textures left
			if (!m_emojiTextures.size())
			{
				m_finished = true;
				if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
				{
					auto filter = sprite->get().getFilter();
					filter.a = 0.0f;
					sprite->get().setFilter(filter);
				}
				return;
			}
			//std::cout << "Swapping Sprites" << std::endl;
			else if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
			{
				if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
				{
					auto filter = sprite->get().getFilter();
					filter.a = 1.0f;
					sprite->get().setFilter(filter);
				}
				//std::cout << "Init Sprite " << sprite->get().getTexturePath();
				std::ostringstream oss;
				oss << "Resources\\Assets\\" << m_emojiTextures.front();

				if (!m_emojiTextures.size())
					oss << "Blank.png";

				sprite->get().setTexturePath(oss.str());
				m_emojiTextures.pop();
				//std::cout << "End Sprite " << sprite->get().getTexturePath() << std::endl;
			}

			m_timer = m_delayBetweenEmojis;
		}
	}

	void EmojiScript::OnTriggerEnter(Entity otherEnt)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (otherEnt != PLAYER_STATUS.GetPlayerEntity() || m_activated)
			return;

		m_activated = true;
		m_timer = 0.0f;
	}

}