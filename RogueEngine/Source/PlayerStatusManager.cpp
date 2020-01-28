#include "Precompiled.h"
#include "PlayerStatusManager.h"

namespace Rogue
{
	PlayerStatusManager::PlayerStatusManager(): 
		m_isLightMode{ true },
		m_maxJumpTimer{0.5f},
		m_hitchhikedEntity{ static_cast<Entity>(-1) },
		m_inLightDur{0.0f}
	{
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

	void PlayerStatusManager::ToggleLightStatus()
	{
		m_isLightMode = !m_isLightMode;
	}

	void PlayerStatusManager::SetLightStatus(bool isLight)
	{
		m_isLightMode = isLight;
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

}
