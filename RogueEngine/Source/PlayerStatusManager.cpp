#include "Precompiled.h"
#include "PlayerStatusManager.h"
#include "GameEvent.h"
#include "EventDispatcher.h"

namespace Rogue
{
	PlayerStatusManager::PlayerStatusManager():
		m_entity{ MAX_ENTITIES },
		m_indicator{MAX_ENTITIES},
		m_isLightMode{ true },
		m_hasJumped{ false },
		m_inSlowMo{false},
		m_maxJumpTimer{0.5f},
		m_hitchhikedEntity{ MAX_ENTITIES },
		m_inLightDur{0.0f},
		m_maxTeleportCharge {3.0f},
		m_teleportCharge{ 3.0f },
		m_teleportDelayTimer {0.0f}
	{
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
	}

	Entity PlayerStatusManager::GetIndicator() const
	{
		return Entity();
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

	void PlayerStatusManager::SetSlowMo(bool slowMo)
	{
		m_inSlowMo = slowMo;
	}

	bool PlayerStatusManager::InSlowMo() const
	{
		return m_inSlowMo;
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

	void PlayerStatusManager::SetHitchhikeEntity(Entity hitchhiked)
	{
		m_hitchhikedEntity = hitchhiked;
	}

	Entity PlayerStatusManager::GetHitchhikedEntity() const
	{
		return m_hitchhikedEntity;
	}

	void PlayerStatusManager::ChangePlayerSprite()
	{
		EntChangeSpriteEvent* ev;
		
		if (m_isLightMode)
			ev = new EntChangeSpriteEvent(m_entity, "Resources\\Assets\\ExaIdle.png");
		else
			ev = new EntChangeSpriteEvent(m_entity, "Resources\\Assets\\ElaIdle.png");

		ev->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
		EventDispatcher::instance().AddEvent(ev);
	}

}
