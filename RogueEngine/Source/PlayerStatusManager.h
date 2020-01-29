#pragma once
#include "Types.h"
#define PLAYER_STATUS PlayerStatusManager::instance()

namespace Rogue
{
	class PlayerStatusManager
	{
	public:

		static PlayerStatusManager& instance()
		{
			static PlayerStatusManager instance;
			return instance;
		}

		PlayerStatusManager();

		//Jump
		float GetJumpMaxTimer() const;

		//Physical Status
		void SetInLightDur(float duration = 0.16f);
		float GetInLightDur() const;

		//Teleporting
		float GetTeleportCharge() const;
		float GetMaxTeleportCharge() const;
		void IncrementTeleportCharge(float duration);
		void SetTeleportCharge(float duration);
		void DecrementTeleportDelay(float duration);
		void SetTeleportDelay(float duration);
		float GetTeleportDelay() const;

		//Light/Dark mode
		void ToggleLightStatus();
		void SetLightStatus(bool isLight);
		bool GetLightStatus() const;

		//Hitchhike
		void SetHitchhikeEntity(Entity ent);
		Entity GetHitchhikedEntity() const;


	private:

		//Jumping mechanic
		const float m_maxJumpTimer;

		//Physical status
		float m_inLightDur;

		//Teleporting
		float m_teleportCharge;
		float m_maxTeleportCharge;
		float m_teleportDelayTimer;

		//Timed Entities


		//Light/Dark mode mechanic
		bool m_isLightMode;

		//Hitchhiking mechanic
		Entity m_hitchhikedEntity;
	};
}