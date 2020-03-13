#include "Precompiled.h"
#include "EmojiAndZoomScript.h"

namespace Rogue
{
	EmojiAndZoomScript::EmojiAndZoomScript(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: TriggerZoom(entity, logicComponent, statsComponent),
		m_activated{ false },
		m_finished{ false },
		m_delayBetweenEmojis{ statsComponent.GetEmojiDelay() },
		m_timer{ statsComponent.GetZoomDuration() }
	{
		m_delayBetweenEmojis = m_statsComponent->GetEmojiDelay();

		for (auto textureStr : m_statsComponent->GetEmojiTextures())
		{
			m_emojiTextures.push(textureStr);
		}

		if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_entity))
		{
			auto filter = sprite->get().getFilter();
			filter.a = 0.0f;
			sprite->get().setFilter(filter);
		}
	}

	void EmojiAndZoomScript::AIIdleUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		TriggerZoom::AIIdleUpdate(); 
		
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
				PLAYER_STATUS.UnfreezeControls();

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
				if (m_emojiTextures.size())
				{
					oss << "Resources\\Assets\\" << m_emojiTextures.front();
					sprite->get().setTexturePath(oss.str());
					m_emojiTextures.pop();
				}
			}

			m_timer = m_delayBetweenEmojis;
		}
	}
	
	void EmojiAndZoomScript::OnTriggerEnter(Entity otherEnt)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;
	
		if (PLAYER_STATUS.GetTriggerOnce())
		{
			return;
		}
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(otherEnt))
		{
			if (g_engine.m_coordinator.GetHierarchyInfo(m_entity).m_tag == "TriggerOnce")
			{
				PLAYER_STATUS.SetTriggerOnce(true);
			}
		}
		TriggerZoom::OnTriggerEnter(otherEnt);

		//This part is copied from EmojiScript
		if (otherEnt != PLAYER_STATUS.GetPlayerEntity() || m_activated)
			return;

		m_activated = true;
		m_timer = m_statsComponent->GetZoomDuration();			//Set to Zoom duration so wait for zoom to be finished
		PLAYER_STATUS.FreezeControls();
	}
}