#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include "Vector2D.h"

#define FORCE_FACTOR 100000
#define POSITION_RELATIVITY 100

namespace Rogue
{
	struct TimedEntity
	{
		Entity m_entity;
		float m_durationLeft;

		TimedEntity(Entity entity, float duration = 1.0f)
			: m_entity{ entity }, m_durationLeft{ duration }{}

	};

	class PlayerControllerSystem
		: public System, public EventListener
	{
	public:

		PlayerControllerSystem();
		~PlayerControllerSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event * ev) override;
		void Shutdown() override;

		std::vector<TimedEntity> GetTimedEntities() const;
		void AddToTimedEntities(TimedEntity newEnt);
		void AddToTimedEntities(Entity entity, float duration = 1.0f);
		void ClearTimedEntities();

		void CreateTeleportEvent(Vec2 newPosition);

	private:
		void CreateBallAttack();
		std::vector<TimedEntity> m_timedEntities;
		float m_timer;
	};
}