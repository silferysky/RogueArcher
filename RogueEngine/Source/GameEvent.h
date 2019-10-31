#pragma once
#include "Event.h"
#include "Vector2D.h"

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

		SET_EVENT_CATEGORY(EventCatEntChangeState)
			SET_EVENT_TYPE(EvEntityChangeState)

		EntChangeStateEvent(int id, int newState)
			: EntityEvent(id), EnemyState(newState) {}

		inline int GetState() { return EnemyState; }

	private:
		int EnemyState;
	};

	class EntTeleportEvent : public EntityEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatEntMove)
		SET_EVENT_TYPE(EvEntityTeleport)

		EntTeleportEvent(int id, bool forceAffected)
			:EntityEvent(id), AffectedByForce(forceAffected) {}
		virtual ~EntTeleportEvent() = default;

		inline bool isAffectedByForce() { return AffectedByForce; }

	protected:
		bool AffectedByForce;
	};

	class EntMoveEvent : public EntTeleportEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatEntMove)
		SET_EVENT_TYPE(EvEntityMove)

		EntMoveEvent(int id, bool forceAffected, float x, float y)
			: EntTeleportEvent(id, forceAffected), moveVector{Vec2(x,y)} {}
		EntMoveEvent(int id, bool forceAffected, Vec2 vec)
			: EntTeleportEvent(id, forceAffected), moveVector{ vec } {}

		inline float GetXMovement() { return moveVector.x; }
		inline float GetYMovement() { return moveVector.y; }

	private:
		Vec2 moveVector;
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

		SET_EVENT_TYPE(EvEntityAttacking)

		EntAttackingEvent(int id, int damage)
			: EntAttackEvent(id, damage) {}

	};

	class EntDamagedEvent : public EntAttackEvent
	{
	public:

		SET_EVENT_TYPE(EvEntityDamaged)

		EntDamagedEvent(int id, int damage)
			: EntAttackEvent(id, damage) {}

	};

	class EntDestroyEvent : public EntityEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatEntDestroy)
		SET_EVENT_TYPE(EvEntityDestroy)

		EntDestroyEvent(int id)
			: EntityEvent(id) {}

	};

	class EntCollisionEvent : public EntityEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnCollision)

		EntCollisionEvent(int id_1, int id_2)
			:EntityEvent(id_1), collidedEntity{id_2}{}

		inline int GetOtherEntity() { return collidedEntity; }

	private:
		int collidedEntity;
	};

	class EntTriggeredEvent : public EntityEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnTrigger)

		EntTriggeredEvent(int id_1, int id_2)
			:EntityEvent(id_1), collidedEntity{ id_2 }{}

		inline int GetOtherEntity() { return collidedEntity; }

	private:
		int collidedEntity;
	};
}
