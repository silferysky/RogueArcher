#pragma once
#include "Event.h"
#include "Vector2D.h"
#include "Types.h"
#include "Vector2D.h"

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

	class EntChangeStateEvent : public EntityEvent
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
			:EntityEvent(id), moveVector{ Vec2(x,y) } {}
		EntTeleportEvent(Entity id, Vec2 vec)
			:EntityEvent(id), moveVector{ vec } {}

		virtual ~EntTeleportEvent() = default;

		inline Vec2 GetVecMovement() { return moveVector; }

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

		inline Vec2 GetVecMovement() { return moveVector; }
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

	class EntCollisionOrTrigger : public EntityEvent
	{
	public:
		EntCollisionOrTrigger(Entity id_1, Entity id_2)
			:EntityEvent(id_1), collidedEntity{ id_2 }{}

		inline Entity GetOtherEntity() { return collidedEntity; }

	protected:
		Entity collidedEntity;
	};

	class EntCollisionEnterEvent : public EntCollisionOrTrigger
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnCollisionEnter)

		EntCollisionEnterEvent(Entity id_1, Entity id_2)
			:EntCollisionOrTrigger(id_1, id_2){}
	};

	class EntCollisionStayEvent : public EntCollisionOrTrigger
	{
		Vec2 m_posA;
		Vec2 m_posB;
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
			SET_EVENT_TYPE(EvOnCollisionStay)

		EntCollisionStayEvent(Entity id_1, Entity id_2, Vec2 posA, Vec2 posB) :
			EntCollisionOrTrigger(id_1, id_2), m_posA{ posA }, m_posB{ posB }
		{}

		Vec2 GetAPos() const
		{
			return m_posA;
		}

		Vec2 GetBPos() const
		{
			return m_posB;
		}
	};

	class EntCollisionExitEvent : public EntCollisionOrTrigger
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnCollisionExit)

		EntCollisionExitEvent(Entity id_1, Entity id_2)
			:EntCollisionOrTrigger(id_1, id_2) {}
	};

	class EntTriggerEnterEvent : public EntCollisionEnterEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatTrigger)
		SET_EVENT_TYPE(EvOnCollisionEnter)

		EntTriggerEnterEvent(Entity id_1, Entity id_2)
			:EntCollisionEnterEvent(id_1, id_2) {}
	};

	class EntTriggerStayEvent : public EntCollisionStayEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatTrigger)
		SET_EVENT_TYPE(EvOnCollisionStay)

		EntTriggerStayEvent(Entity id_1, Entity id_2, Vec2 transA, Vec2 transB)
			:EntCollisionStayEvent(id_1, id_2, transA, transB) {}
	};

	class EntTriggerExitEvent : public EntCollisionExitEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatTrigger)
		SET_EVENT_TYPE(EvOnCollisionExit)

		EntTriggerExitEvent(Entity id_1, Entity id_2)
			:EntCollisionExitEvent(id_1, id_2) {}
	};

	class EntPickedEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityPicked)

		EntPickedEvent(Entity _id) :
			EntityEvent(_id)
		{}
	};
}
