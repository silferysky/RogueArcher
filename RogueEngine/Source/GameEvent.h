#pragma once
#include "Event.h"
#include "Vector2D.h"
#include "Types.h"

namespace Rogue
{
	class EntityEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatEntity)

		inline int GetEntityID() { return ID; }

	protected:
		EntityEvent(Entity id)
			: ID(id) {}

		Entity ID;
	};

	class EntChangeStateEvent : EntityEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatEntChangeState)
			SET_EVENT_TYPE(EvEntityChangeState)

		EntChangeStateEvent(Entity id, int newState)
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

		EntTeleportEvent(Entity id, float x, float y)
			:EntityEvent(id) {}
		EntTeleportEvent(Entity id, Vec2 vec)
			:EntityEvent(id) {}

		virtual ~EntTeleportEvent() = default;

		inline Vec2 GetVecmovement() { return moveVector; }

	protected:
		Vec2 moveVector;
	};

	class EntMoveEvent : public EntityEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatEntMove)
		SET_EVENT_TYPE(EvEntityMove)

		EntMoveEvent(Entity id, bool forceAffected, float x, float y)
			: EntityEvent(id), AffectedByForce{ forceAffected }, moveVector{ Vec2(x,y) } {}
		EntMoveEvent(Entity id, bool forceAffected, Vec2 vec)
			: EntityEvent(id), AffectedByForce{ forceAffected }, moveVector{ vec } {}
		virtual ~EntMoveEvent() = default;

		inline Vec2 GetVecmovement() { return moveVector; }
		inline bool isAffectedByForce() { return AffectedByForce; }

	private:
		bool AffectedByForce;
		Vec2 moveVector;
	};

	class EntAttackEvent : public EntityEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatEntAttack)

		inline int GetDamage() { return Damage; }

	protected:
		EntAttackEvent(Entity id, int damage)
			: EntityEvent(id), Damage(damage) {}

		int Damage;
	};

	class EntAttackingEvent : public EntAttackEvent
	{
	public:

		SET_EVENT_TYPE(EvEntityAttacking)

		EntAttackingEvent(Entity id, int damage)
			: EntAttackEvent(id, damage) {}

	};

	class EntDamagedEvent : public EntAttackEvent
	{
	public:

		SET_EVENT_TYPE(EvEntityDamaged)

		EntDamagedEvent(Entity id, int damage)
			: EntAttackEvent(id, damage) {}

	};

	class EntDestroyEvent : public EntityEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatEntDestroy)
		SET_EVENT_TYPE(EvEntityDestroy)

		EntDestroyEvent(Entity id)
			: EntityEvent(id) {}

	};

	class EntCollisionEvent : public EntityEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnCollision)

		EntCollisionEvent(Entity id_1, Entity id_2)
			:EntityEvent(id_1), collidedEntity{id_2}{}

		inline Entity GetOtherEntity() { return collidedEntity; }

	private:
		Entity collidedEntity;
	};

	class EntTriggeredEvent : public EntityEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnTrigger)

		EntTriggeredEvent(Entity id_1, Entity id_2)
			:EntityEvent(id_1), collidedEntity{ id_2 }{}

		inline Entity GetOtherEntity() { return collidedEntity; }

	private:
		Entity collidedEntity;
	};
}
