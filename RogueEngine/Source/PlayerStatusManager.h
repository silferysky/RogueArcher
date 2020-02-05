#pragma once
#include "Types.h"
#include "Vector2D.h"
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

		void Reset();
		void SetRunCount(size_t count);
		size_t GetRunCount();

		//Player Entity
		void SetPlayerEntity(Entity ent);
		Entity GetPlayerEntity() const;
		void SetIndicator(Entity ent);
		Entity GetIndicator() const;

		void SetPlayerStartingPosition(const Vec2& pos);
		Vec2 GetPlayerStartingPosition() const;

		//Jump
		void SetHasJumped(bool jumped);
		bool HasJumped() const;
		float GetJumpMaxTimer() const;

		//Physical Status
		void SetInLightDur(float duration = 0.16f);
		float GetInLightDur() const;
		void SetIndicatorStatus(bool showIndicator = true);
		bool ShowIndicator() const;

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

		//Checkpoint
		void SetCheckpoint(Vec2 checkpoint);
		Vec2 GetCheckpoint() const;
		void SetStartingPos(Vec2 startingPos);
		Vec2 GetStartingPos() const;

	private:

		void ChangePlayerSprite();

		size_t m_runCount;

		Entity m_entity;
		Entity m_indicator;
		Vec2 m_playerStartPos;

		//Jumping mechanic
		bool m_hasJumped;
		const float m_maxJumpTimer;

		//Physical status
		float m_inLightDur;
		bool m_indicatorShown;

		//Teleporting
		float m_teleportCharge;
		float m_maxTeleportCharge;
		float m_teleportDelayTimer;


		//Light/Dark mode mechanic
		bool m_isLightMode;

		//Hitchhiking mechanic
		Entity m_hitchhikedEntity;

		//Checkpoint
		Vec2 m_checkpoint;
		Vec2 m_startingPos;
	};
}