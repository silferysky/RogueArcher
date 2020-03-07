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
#include "BoxCollider2DComponent.h"
#include "Vector2D.h"
#include "PhysicsDataStructures.hpp"

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

	template <typename TColliderA, typename TColliderB>
	class EntCollisionOrTrigger : public Event
	{
	public:
		inline CollisionInfo<TColliderA>& GetThis() { return m_this; }
		inline CollisionInfo<TColliderB>& GetOther() { return m_other; }

		template <typename TColliderA, typename TColliderB>
		EntCollisionOrTrigger(const CollisionInfo<TColliderA>& colA, const CollisionInfo<TColliderB>& colB) :
			m_this{ colA },
			m_other{ colB }
		{}

	protected:
		CollisionInfo<TColliderA> m_this;
		CollisionInfo<TColliderB> m_other;
	};
	
	template <typename TColliderA, typename TColliderB>
	class EntCollisionEnterEvent : public EntCollisionOrTrigger<TColliderA, TColliderB>
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnCollisionEnter)

		template <typename TColliderA, typename TColliderB>
		EntCollisionEnterEvent(const CollisionInfo<TColliderA>& colA, const CollisionInfo<TColliderB>& colB) :
			EntCollisionOrTrigger<TColliderA, TColliderB>(colA, colB)
		{}
	};

	template <typename TColliderA, typename TColliderB>
	class EntCollisionStayEvent : public EntCollisionOrTrigger<TColliderA, TColliderB>
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnCollisionStay)

		template <typename TColliderA, typename TColliderB>
		EntCollisionStayEvent(const CollisionInfo<TColliderA>& colA, const CollisionInfo<TColliderB>& colB) :
			EntCollisionOrTrigger<TColliderA, TColliderB>(colA, colB)
		{}
	};

	template <typename TColliderA, typename TColliderB>
	class EntCollisionExitEvent : public EntCollisionOrTrigger<TColliderA, TColliderB>
	{
	public:
		SET_EVENT_CATEGORY(EventCatCollision)
		SET_EVENT_TYPE(EvOnCollisionExit)

		template <typename TColliderA, typename TColliderB>
		EntCollisionExitEvent(const CollisionInfo<TColliderA>& colA, const CollisionInfo<TColliderB>& colB) :
			EntCollisionOrTrigger<TColliderA, TColliderB>(colA, colB)
		{}
	};

	template <typename TColliderA, typename TColliderB>
	class EntTriggerEnterEvent : public EntCollisionEnterEvent<TColliderA, TColliderB>
	{
	public:
		SET_EVENT_CATEGORY(EventCatTrigger)

		template <typename TColliderA, typename TColliderB>
		EntTriggerEnterEvent(const CollisionInfo<TColliderA>& colA, const CollisionInfo<TColliderB>& colB) :
			EntCollisionEnterEvent<TColliderA, TColliderB>(colA, colB)
		{}
	};

	template <typename TColliderA, typename TColliderB>
	class EntTriggerStayEvent : public EntCollisionStayEvent<TColliderA, TColliderB>
	{
	public:
		SET_EVENT_CATEGORY(EventCatTrigger)

		template <typename TColliderA, typename TColliderB>
		EntTriggerStayEvent(const CollisionInfo<TColliderA>& colA, const CollisionInfo<TColliderB>& colB) :
			EntCollisionStayEvent<TColliderA, TColliderB>(colA, colB)
		{}
	};

	template <typename TColliderA, typename TColliderB>
	class EntTriggerExitEvent : public EntCollisionExitEvent<TColliderA, TColliderB>
	{
	public:
		SET_EVENT_CATEGORY(EventCatTrigger)

		template <typename TColliderA, typename TColliderB>
		EntTriggerExitEvent(const CollisionInfo<TColliderA>& colA, const CollisionInfo<TColliderB>& colB) :
			EntCollisionExitEvent<TColliderA, TColliderB>(colA, colB)
		{}
	};

	using AABBCollisionEnterEvent = EntCollisionEnterEvent<BoxCollider2DComponent, BoxCollider2DComponent>;
	using AABBCollisionStayEvent = EntCollisionStayEvent<BoxCollider2DComponent, BoxCollider2DComponent>;
	using AABBCollisionExitEvent = EntCollisionExitEvent<BoxCollider2DComponent, BoxCollider2DComponent>;
	using AABBTriggerEnterEvent = EntTriggerEnterEvent<BoxCollider2DComponent, BoxCollider2DComponent>;
	using AABBTriggerStayEvent = EntTriggerStayEvent<BoxCollider2DComponent, BoxCollider2DComponent>;
	using AABBTriggerExitEvent = EntTriggerExitEvent<BoxCollider2DComponent, BoxCollider2DComponent>;

	class EntPickedEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityPicked)

		EntPickedEvent(Entity _id) :
			EntityEvent(_id) {}
	};

	class EntHoverEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityHover)

		EntHoverEvent(Entity _id) :
			EntityEvent(_id){}
	};

	class EntHitchhikeEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityHitchhike)

		EntHitchhikeEvent(Entity _id)
			: EntityEvent(_id){}
	};

	class EntResetToCameraEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvEntityResetToCamera)

		EntResetToCameraEvent(Entity _id)
			: EntityEvent(_id) {}
	};

	class FreezeControlEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvFreezeControls);

		FreezeControlEvent(Entity _id)
			: EntityEvent(_id) {}
	};

	class UnfreezeControlEvent : public EntityEvent
	{
	public:
		SET_EVENT_TYPE(EvUnfreezeControls);

		UnfreezeControlEvent(Entity _id)
			: EntityEvent(_id) {}
	};
}
