/*Start Header*************************************************************************/
/*!
\file           GameEvent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for GameEvent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Event.h"
#include "Vector2D.h"
#include "Types.h"
#include "Vector2D.h"

namespace Rogue
{
	class ResetGameEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatEntity);
		SET_EVENT_TYPE(EvResetGame);

		ResetGameEvent() {}
	};

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

	class EntChangeSpriteEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityChangeSprite);

		EntChangeSpriteEvent(Entity ent, std::string tex, std::string path = "")
			: EntityEvent(ent), file{ tex } {}

		inline std::string GetFile() { return file; }
		inline std::string GetPath() { return path + file; }

	private:
		std::string file;
		std::string path;
	};

	class EntChangeRGBAEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityChangeRGBA);

		EntChangeRGBAEvent(Entity ent, float R, float G, float B, float A = 1.0f)
			: EntityEvent(ent), r{ R }, g{ G }, b{ B }, a{ A }{}

		inline float R() const { return r; }
		inline float G() const { return g; }
		inline float B() const { return b; }
		inline float A() const { return a; }

	private:
		float r, g, b, a;
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
		Vec2 m_scaleA;
		Vec2 m_scaleB;
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
			SET_EVENT_TYPE(EvOnCollisionStay)

			EntCollisionStayEvent(Entity id_1, Entity id_2, Vec2 posA, Vec2 posB, Vec2 scaleA, Vec2 scaleB) :
			EntCollisionOrTrigger(id_1, id_2), m_posA{ posA }, m_posB{ posB }, m_scaleA{ scaleA }, m_scaleB{ scaleB }
		{}

		Vec2 GetAPos() const
		{
			return m_posA;
		}

		Vec2 GetBPos() const
		{
			return m_posB;
		}

		Vec2 GetScaleA() const
		{
			return m_scaleA;
		}

		Vec2 GetScaleB() const
		{
			return m_scaleB;
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

		EntTriggerStayEvent(Entity id_1, Entity id_2, Vec2 transA, Vec2 transB) :
			EntCollisionStayEvent(id_1, id_2, transA, transB, Vec2(), Vec2()) {}
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

	class EntHitchhikeEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityHitchhike)

		EntHitchhikeEvent(Entity _id)
			: EntityEvent(_id){}
	};
}
