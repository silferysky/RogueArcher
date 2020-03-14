#include "Precompiled.h"
#include "PlayerStatusManager.h"
#include "GameEvent.h"
#include "EventDispatcher.h"

namespace Rogue
{
	PlayerStatusManager::PlayerStatusManager() :
		m_runCount{ 0 },
		m_entity{ MAX_ENTITIES },
		m_indicator{ MAX_ENTITIES },
		m_hitchhikeIndicator{ MAX_ENTITIES },
		m_playerStartPos{ Vec2(0,0) },
		m_isActive{ true },
		m_freezeControlDuration{ -1.0f },
		m_isLightMode{ true },
		m_hasJumped{ false },
		m_moveLeft{ false },
		m_indicatorShown{ true },
		m_death{ false },
		m_maxJumpTimer{ 0.5f },
		m_hitchhikedEntity{ MAX_ENTITIES },
		m_hitchhikableEntity{ MAX_ENTITIES },
		m_inLightDur{ 0.0f },
		m_maxTeleportCharge{ 3.0f },
		m_teleportCharge{ 3.0f },
		m_teleportDelayTimer{ 0.0f },
		m_startingPos{ 0.0f, 0.0f },
		m_checkpoint{ 0.0f, 0.0f },
		m_lastLevel { "None"},
		m_isEnding{ false },
		m_infiniteJumps{ false },
		m_triggerOnce(false),
		m_endTrigger{ false },
		m_currLevel{ LEVEL::TUTORIAL }
	{

	}

	void PlayerStatusManager::Reset()
	{
		m_entity = MAX_ENTITIES;
		m_indicator = MAX_ENTITIES;
		m_isActive = true;
		m_freezeControlDuration = -1.0f;
		m_isLightMode = true;
		m_hasJumped = false;
		m_indicatorShown = true;
		m_death = false;
		m_hitchhikedEntity = MAX_ENTITIES;
		m_hitchhikableEntity = MAX_ENTITIES;
		m_inLightDur = 0.0f;
		m_teleportCharge = 3.0f;
		m_teleportDelayTimer = 0.0f;
		m_startingPos = { 0.0f, 0.0f };
		m_checkpoint = { 0.0f, 0.0f };
		m_souls.clear();

		++m_runCount;
	}

	void PlayerStatusManager::SetRunCount(size_t count)
	{
		m_runCount = count;
	}

	size_t PlayerStatusManager::GetRunCount()
	{
		return m_runCount;
	}

	void PlayerStatusManager::SetPlayerEntity(Entity ent)
	{
		m_entity = ent;
	}

	Entity PlayerStatusManager::GetPlayerEntity() const
	{
		return m_entity;
	}

	void PlayerStatusManager::SetIndicator(Entity ent)
	{
		m_indicator = ent;
	}

	Entity PlayerStatusManager::GetIndicator() const
	{
		return m_indicator;
	}

	void PlayerStatusManager::SetPlayerActive(bool active)
	{
		m_isActive = active;
	}

	bool PlayerStatusManager::IsPlayerActive() const
	{
		return m_isActive;
	}

	void PlayerStatusManager::SetFreezeControlTimer(float time)
	{
		m_freezeControlDuration = time;
		FreezeControlEvent freeze{ MAX_ENTITIES };
		freeze.SetSystemReceivers(static_cast<int>(SystemID::id_PLAYERCONTROLLERSYSTEM));
		EventDispatcher::instance().AddEvent(freeze);
	}

	float PlayerStatusManager::GetFreezeControlTimer() const
	{
		return m_freezeControlDuration;
	}

	void PlayerStatusManager::FreezeControls()
	{
		m_freezeControlDuration = 10000.0f;

		FreezeControlEvent freeze{ MAX_ENTITIES };
		freeze.SetSystemReceivers(static_cast<int>(SystemID::id_PLAYERCONTROLLERSYSTEM));
		EventDispatcher::instance().AddEvent(freeze);
	}

	void PlayerStatusManager::UnfreezeControls()
	{
		m_freezeControlDuration = -1.0f;

		UnfreezeControlEvent unfreeze{ MAX_ENTITIES };
		unfreeze.SetSystemReceivers(static_cast<int>(SystemID::id_PLAYERCONTROLLERSYSTEM));
		EventDispatcher::instance().AddEvent(unfreeze);
	}

	void PlayerStatusManager::SetHasJumped(bool jumped)
	{
		m_hasJumped = jumped;
	}

	bool PlayerStatusManager::HasJumped() const
	{
		return m_hasJumped;
	}

	float PlayerStatusManager::GetJumpMaxTimer() const
	{
		return m_maxJumpTimer;
	}

	void PlayerStatusManager::SetInLightDur(float duration)
	{
		m_inLightDur = duration;
	}

	float PlayerStatusManager::GetInLightDur() const
	{
		return m_inLightDur;
	}

	void PlayerStatusManager::SetIndicatorStatus(bool showIndicator)
	{
		m_indicatorShown = showIndicator;
	}

	bool PlayerStatusManager::ShowIndicator() const
	{
		return m_indicatorShown;
	}

	void PlayerStatusManager::SetDeath(bool death)
	{
		m_death = death;
	}

	bool PlayerStatusManager::GetDeath() const
	{
		return m_death;
	}

	void PlayerStatusManager::SetMoveLeft(bool isLeft)
	{
		m_moveLeft = isLeft;
	}

	bool PlayerStatusManager::GetMoveLeft() const
	{
		return m_moveLeft;
	}

	void PlayerStatusManager::DestroyIndicators()
	{
		if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES)
		{
			PLAYER_STATUS.SetIndicatorStatus(false);
			g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetIndicator());
			g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetHitchhikeIndicator());
			PLAYER_STATUS.SetIndicator(MAX_ENTITIES);
			PLAYER_STATUS.SetHitchhikeIndicator(MAX_ENTITIES);
		}
	}

	float PlayerStatusManager::GetTeleportCharge() const
	{
		return m_teleportCharge;
	}

	float PlayerStatusManager::GetMaxTeleportCharge() const
	{
		return m_maxTeleportCharge;
	}

	void PlayerStatusManager::IncrementTeleportCharge(float duration)
	{
		m_teleportCharge += duration;
	}

	void PlayerStatusManager::SetTeleportCharge(float duration)
	{
		m_teleportCharge = duration;
	}

	void PlayerStatusManager::DecrementTeleportDelay(float duration)
	{
		m_teleportDelayTimer -= duration;
	}

	void PlayerStatusManager::SetTeleportDelay(float duration)
	{
		m_teleportDelayTimer = duration;
	}

	float PlayerStatusManager::GetTeleportDelay() const
	{
		return m_teleportDelayTimer;
	}

	void PlayerStatusManager::ToggleLightStatus()
	{
		m_isLightMode = !m_isLightMode;
		ChangePlayerSprite();

	}

	void PlayerStatusManager::SetLightStatus(bool isLight)
	{
		m_isLightMode = isLight;
		ChangePlayerSprite();
	}

	bool PlayerStatusManager::GetLightStatus() const
	{
		return m_isLightMode;
	}

	void PlayerStatusManager::SetHitchhikedEntity(Entity hitchhiked)
	{
		m_hitchhikedEntity = hitchhiked;
	}

	Entity PlayerStatusManager::GetHitchhikedEntity() const
	{
		return m_hitchhikedEntity;
	}

	void PlayerStatusManager::SetHitchhikeIndicator(Entity ent)
	{
		m_hitchhikeIndicator = ent;
	}

	Entity PlayerStatusManager::GetHitchhikeIndicator() const
	{
		return m_hitchhikeIndicator;
	}

	void PlayerStatusManager::SetHitchhikableEntity(Entity ent)
	{
		m_hitchhikableEntity = ent;
	}

	Entity PlayerStatusManager::GetHitchhikableEntity()
	{
		return m_hitchhikableEntity;
	}

	void PlayerStatusManager::SetCheckpoint(Vec2 checkpoint)
	{
		m_checkpoint = checkpoint;
	}

	Vec2 PlayerStatusManager::GetCheckpoint() const
	{
		return m_checkpoint;
	}

	void PlayerStatusManager::SetStartingPos(Vec2 startingPos)
	{
		m_startingPos = startingPos;
	}

	Vec2 PlayerStatusManager::GetStartingPos() const
	{
		return m_startingPos;
	}

	bool PlayerStatusManager::GetTriggerOnce() const
	{
		return m_triggerOnce;
	}

	void PlayerStatusManager::SetTriggerOnce(bool trigger)
	{
		m_triggerOnce = trigger;
	}

	//================================================================
	//							SOUL COLLECTIBLE
	//================================================================

	size_t PlayerStatusManager::GetCollectedSoulsInLevel()
	{
		size_t collected = 0;

		// If didn't exist, inserts the level, returns nullptr
		if (m_souls[m_currLevel] == nullptr)
			return 0;

		for (Soul& soul : *m_souls[m_currLevel])
		{
			if (soul.m_collected)
				++collected;
		}
		return collected;
	}

	size_t PlayerStatusManager::GetCollectedSoulsInLevel(LEVEL level)
	{
		size_t collected = 0;
		
		// If didn't exist, inserts the level, returns nullptr
		if (m_souls[level] == nullptr)
			return 0;

		for (Soul& soul : *m_souls[level])
		{
			if (soul.m_collected)
				++collected;
		}
		return collected;
	}

	size_t PlayerStatusManager::GetCollectedSoulsInGame()
	{
		size_t collected = 0;

		for (auto& pair : m_souls)
		{
			for (Soul& soul : *pair.second)
			{
				if (soul.m_collected)
					++collected;
			}
		}
		return collected;
	}

	size_t PlayerStatusManager::GetTotalSoulsInLevel()
	{
		// If didn't exist, inserts the level, returns nullptr
		if (m_souls[m_currLevel] == nullptr)
		{
			std::cout << "Total Souls: Level " << m_currLevel << " not registered!" << std::endl;
			return 0;
		}
		return m_souls[m_currLevel]->size();
	}

	size_t PlayerStatusManager::GetTotalSoulsInLevel(LEVEL level)
	{
		if(m_souls[m_currLevel] == nullptr)
			std::cout << "Total Souls: Level " << level << " not registered!" << std::endl;
		
		return m_souls[m_currLevel]->size();
	}

	size_t PlayerStatusManager::GetTotalSoulsInGame()
	{
		size_t numSouls = 0;

		for (auto& pair : m_souls)
			numSouls += pair.second->size();

		return numSouls;
	}

	void PlayerStatusManager::CollectSoul(Entity entity)
	{
		if (m_souls[m_currLevel] == nullptr)
		{
			std::cout << "Curr level " << m_currLevel << " not in map!" << std::endl;
			return;
		}

		for (Soul& soul : *m_souls[m_currLevel])
		{
			std::stringstream ss;
			ss << " Soul entity: " << entity << " Current soul: " << soul.m_entity << std::boolalpha << "Collected? " << soul.m_collected;
			RE_INFO(ss.str());

			if (entity == soul.m_entity)
				soul.m_collected = true;
		}
	}

	void PlayerStatusManager::AddSoul(Entity entity)
	{
		if (m_souls[m_currLevel] == nullptr)
			return;

		m_souls[m_currLevel]->emplace_back(Soul(entity));
	}

	bool PlayerStatusManager::RegisterLevel(LEVEL level)
	{
		 std::pair<std::map<LEVEL, std::shared_ptr<std::vector<Soul>>>::iterator, bool> it =  m_souls.insert(std::make_pair(level, std::make_shared<std::vector<Soul>>()));
		 
		 return it.second;
	}

	void PlayerStatusManager::RemoveCollectedSouls()
	{
		if (m_souls[m_currLevel] == nullptr)
			return;

		for (Soul& soul : *m_souls[m_currLevel])
		{
			if (soul.m_collected && g_engine.m_coordinator.ComponentExists<SoulComponent>(soul.m_entity))
				g_engine.m_coordinator.AddToDeleteQueue(soul.m_entity);
		}
	}

	//================================================================

	void PlayerStatusManager::SetLastLevel(std::string_view str)
	{
		m_lastLevel = str.data();
	}

	std::string_view PlayerStatusManager::GetLastLevel() const
	{
		return m_lastLevel;
	}

	void PlayerStatusManager::SetCurrLevel(LEVEL level)
	{
		m_currLevel = level;
	}

	LEVEL PlayerStatusManager::GetCurrLevel() const
	{
		return m_currLevel;
	}

	void PlayerStatusManager::ResetEndGame()
	{
		SetEnding(false);
		m_souls.clear();
	}

	void PlayerStatusManager::SetEnding(bool ending)
	{
		m_isEnding = ending;
	}

	bool PlayerStatusManager::GetEnding() const
	{
		return m_isEnding;
	}

	void PlayerStatusManager::SetInfiniteJumps(bool infinite)
	{
		m_infiniteJumps = infinite;
	}

	bool PlayerStatusManager::GetInfiniteJumps() const
	{
		return m_infiniteJumps;
	}	void PlayerStatusManager::SetEndTrigger(bool willTrigger)
	{
		m_endTrigger = willTrigger;
	}

	bool PlayerStatusManager::GetEndTrigger() const
	{
		return m_endTrigger;
	}

	void PlayerStatusManager::ChangePlayerSprite()
	{
		if (m_isLightMode && m_hitchhikedEntity == MAX_ENTITIES)
		{
			EntChangeSpriteEvent ev(m_entity, "Resources\\Assets\\ExaIdle.png");
			ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);
		}
		else if (!m_isLightMode && m_hitchhikedEntity == MAX_ENTITIES)
		{
			EntChangeSpriteEvent ev(m_entity, "Resources\\Assets\\ElaIdle.png");
			ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);
		}
		else if (m_isLightMode) //&& m_hitchhikedEntity != MAX_ENTITIES
		{
			EntChangeSpriteEvent ev(m_entity, "Resources\\Assets\\ExaIdleHitch.png");
			ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);
		}
		else //!light mode, Hitchhiked != MAX_ENTITIES
		{
			EntChangeSpriteEvent ev(m_entity, "Resources\\Assets\\ElaIdleHitch.png");
			ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);
		}
	}

}
