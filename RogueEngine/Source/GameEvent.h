#pragma once
#include "Event.h"

namespace Rogue
{
	class EntityEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatEntity)

			inline int GetEntityID() { return ID; }

	protected:
		EntityEvent(int id)
			: ID(id) {}

		int ID;
	};

	class EntChangeStateEvent : EntityEvent
	{
	public:
		EntChangeStateEvent(int id, int newState)
			: EntityEvent(id), EnemyState(newState) {}

		SET_EVENT_CATEGORY(EventCatEntChangeState)
			SET_EVENT_TYPE(EvEntityChangeState)

			inline int GetState() { return EnemyState; }

	private:
		int EnemyState;
	};

	class EntTeleportEvent : public EntityEvent
	{
	public:
		EntTeleportEvent(int id, bool forceAffected)
			:EntityEvent(id), AffectedByForce(forceAffected) {}

		virtual ~EntTeleportEvent() = default;

		SET_EVENT_CATEGORY(EventCatEntMove)
			SET_EVENT_TYPE(EvEntityTeleport)

			inline bool isAffectedByForce() { return AffectedByForce; }

	protected:
		bool AffectedByForce;
	};

	class EntMoveEvent : public EntTeleportEvent
	{
	public:
		EntMoveEvent(int id, bool forceAffected, float x, float y)
			: EntTeleportEvent(id, forceAffected), xMovement(x), yMovement(y) {}

		SET_EVENT_CATEGORY(EventCatEntMove)
			SET_EVENT_TYPE(EvEntityMove)

			inline float GetXMovement() { return xMovement; }
		inline float GetYMovement() { return yMovement; }

	private:
		float xMovement, yMovement;
	};

	class EntAttackEvent : public EntityEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatEntAttack)

			inline int GetDamage() { return Damage; }

	protected:
		EntAttackEvent(int id, int damage)
			: EntityEvent(id), Damage(damage) {}

		int Damage;
	};

	class EntAttackingEvent : public EntAttackEvent
	{
	public:
		EntAttackingEvent(int id, int damage)
			: EntAttackEvent(id, damage) {}

		SET_EVENT_TYPE(EvEntityAttacking)
	};

	class EntDamagedEvent : public EntAttackEvent
	{
	public:
		EntDamagedEvent(int id, int damage)
			: EntAttackEvent(id, damage) {}

		SET_EVENT_TYPE(EvEntityDamaged)
	};

	class EntDestroyEvent : public EntityEvent
	{
	public:
		EntDestroyEvent(int id)
			: EntityEvent(id) {}

		SET_EVENT_CATEGORY(EventCatEntDestroy)
			SET_EVENT_TYPE(EvEntityDestroy)
	};
}
