#pragma once
#include "Types.h"
#include "Vector2D.h"
#include "GameLevels.h"
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
		void SetPlayerActive(bool active);
		bool IsPlayerActive() const;

		//Controls
		void SetFreezeControlTimer(float time = 1.0f);
		float GetFreezeControlTimer() const;
		void FreezeControls();
		void UnfreezeControls();

		//Jump
		void SetHasJumped(bool jumped);
		bool HasJumped() const;
		float GetJumpMaxTimer() const;

		//Physical Status
		void SetInLightDur(float duration = 0.16f);
		float GetInLightDur() const;
		void SetIndicatorStatus(bool showIndicator = true);
		bool ShowIndicator() const;
		void SetDeath(bool death);
		bool GetDeath() const;
		void SetMoveLeft(bool isLeft);
		bool GetMoveLeft() const;
		void DestroyIndicators();

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
		void SetHitchhikedEntity(Entity ent);
		Entity GetHitchhikedEntity() const;
		void SetHitchhikeIndicator(Entity ent);
		Entity GetHitchhikeIndicator() const;
		void SetHitchhikableEntity(Entity ent);
		Entity GetHitchhikableEntity();

		//For modifiying sprite based on state
		void ChangePlayerSprite();

		//Checkpoint
		void SetCheckpoint(Vec2 checkpoint);
		Vec2 GetCheckpoint() const;
		void SetStartingPos(Vec2 startingPos);
		Vec2 GetStartingPos() const;
		bool GetTriggerOnce() const;
		void SetTriggerOnce(bool trigger);

		//Collectibles
		size_t GetCollectedSoulsInLevel();
		size_t GetCollectedSoulsInLevel(LEVEL level);
		size_t GetCollectedSoulsInGame();
		size_t GetTotalSoulsInLevel();
		size_t GetTotalSoulsInLevel(LEVEL level);
		size_t GetTotalSoulsInGame();
		void CollectSoul(Entity entity);
		void AddSoul(Entity entity);
		bool RegisterLevel(LEVEL level);
		void RemoveCollectedSouls();

		//Transition
		void SetLastLevel(std::string_view str);
		std::string_view GetLastLevel() const;
		void SetCurrLevel(LEVEL level);
		LEVEL GetCurrLevel() const;

		//Ending
		void ResetEndGame();
		void SetEnding(bool ending);
		bool GetEnding() const;

		//Cheats
		void SetInfiniteJumps(bool infinite = true);
		bool GetInfiniteJumps() const;
		void SetEndTrigger(bool willTrigger = true);
		bool GetEndTrigger() const;
		void SetTrueEndTrigger(bool willTrigger = true);
		bool GetTrueEndTrigger() const;

	private:
		size_t m_runCount;

		Entity m_entity;
		Entity m_indicator;
		Vec2 m_playerStartPos;
		bool m_moveLeft;
		bool m_isActive;

		//Controls
		float m_freezeControlDuration;

		//Jumping mechanic
		bool m_hasJumped;
		const float m_maxJumpTimer;

		//Physical status
		float m_inLightDur;
		bool m_indicatorShown;
		bool m_death;

		//Teleporting
		float m_teleportCharge;
		float m_maxTeleportCharge;
		float m_teleportDelayTimer;

		//Light/Dark mode mechanic
		bool m_isLightMode;

		//Hitchhiking mechanic
		Entity m_hitchhikableEntity;
		Entity m_hitchhikedEntity;
		Entity m_hitchhikeIndicator;

		//Checkpoint
		Vec2 m_checkpoint;
		Vec2 m_startingPos;

		// Collectibles
		std::map<LEVEL, std::unique_ptr<std::vector<Soul>>> m_souls;

		//Level Transition
		std::string m_lastLevel;
		LEVEL m_currLevel;

		//ending
		bool m_isEnding;

		bool m_infiniteJumps;

		bool m_triggerOnce;

		bool m_endTrigger;
		bool m_trueEndTrigger;
	};
}