/*Start Header*************************************************************************/
/*!
\file           CollisionManager.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CollisionManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "REMath.h"
#include "Vector2D.h"
#include "Matrix33.h"
#include "AABB.h"
#include "OBB.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "BoxCollider2DComponent.h"
#include "CircleCollider2DComponent.h"
#include "Manifold.h"
#include "LayerManager.h"
#include "IntegerPairHasher.h"
#include "GameEvent.h"
#include "Main.h"

namespace Rogue
{
	class CollisionManager
	{
	using ColliderPairSet = std::unordered_set<std::pair<Entity, Entity>, IntegerPairHasher<Entity>, IntegerPairComparer<Entity>>;

	ColliderPairSet m_diffPairs; // Stored pairs of aabb and circle
	ColliderPairSet m_boxPairs; // Stored pairs of aabbs
	ColliderPairSet m_circlePairs; // Stored pairs of circles
	std::vector<Manifold> m_manifolds; // To generate and resolve after collision tests

	std::vector<std::pair<Entity, Entity>> m_eraseQueue;

	static const float s_correction_factor;
	static const float s_correction_slop; // Penetration threshold

	public:
		static CollisionManager& instance()
		{
			static CollisionManager instance;
			return instance;
		}

		Mtx33 GetColliderWorldMatrix(const BaseCollider& collider, const TransformComponent& trans) const;
		inline Vec2 GetColliderScale(const BaseCollider& collider, const TransformComponent& trans) const;
		Vec2 GetColliderPosition(const BaseCollider& collider, const TransformComponent& trans) const;
		inline float GetColliderRotation(const BaseCollider& collider, const TransformComponent& trans) const;

		void GenerateManifoldCirclevsCircle(Entity A, Entity B);
		bool GenerateManifoldCirclevsAABB(Entity A, Entity B);
		bool GenerateManifoldAABBvsCircle(Entity A, Entity B);
		void GenerateManifoldAABBvsAABB(Entity A, Entity B);
		void GenerateManifoldAABBvsOBB(Entity A, Entity B);
		void GenerateManifoldOBBvsOBB(Entity A, Entity B);

		CollisionManager() = default;
		~CollisionManager() = default;

		bool DiscreteAABBVsCircle(const AABB& aabb, const BoundingCircle& circle);
		bool DiscreteCircleVsAABB(const BoundingCircle& circle, const AABB& aabb);

		bool DiscretePointVsAABB(const Vec2& point, const AABB& aabb) const;

		// LINE SEGMENT
		bool DiscreteLineVsLine(const LineSegment& lineA, const LineSegment& lineB, Vec2* intersection = nullptr) const;
		bool DiscreteLineVsAABB(const LineSegment& line, const AABB& aabb) const;
		bool DiscreteLineVsAABB(const LineSegment& line, const AABB& aabb, Vec2& intersection) const;

		// BOUNDING CIRCLE
		void UpdateBoundingCircle(BoundingCircle& circle, const TransformComponent& trans) const;
		bool DiscreteCircleVsCircle(const BoundingCircle& circleA, const BoundingCircle& circleB);
		int ContinuousCircleVsLineSegment(const CircleCollider2DComponent& circle, const Vec2& ptEnd, const LineSegment& lineSeg,
			Vec2& interPt, Vec2& normalAtCollision, float& interTime, bool& checkLineEdges);
		int ContinuousCircleVsLineEdge(bool withinBothLines, const CircleCollider2DComponent& circle, const Vec2& ptEnd,
			const LineSegment& lineSeg, Vec2& interPt, Vec2& normalAtCollision, float& interTime);
		int ContinuousCircleVsRay(const Ray& ray, const CircleCollider2DComponent& circle, float& interTime);
		int ContinuousCircleVsCircle(const CircleCollider2DComponent& circleA, const Vec2& velA,
			const CircleCollider2DComponent& circleB, const Vec2& velB, Vec2& interPtA, Vec2& interPtB, float& interTime);
		void ReflectCircleOnLineSegment(const Vec2& ptInter, const Vec2& normal, Vec2& ptEnd, Vec2& reflected);
		void ReflectCircleOnPillar(const Vec2& normal, const float& interTime, const Vec2& ptStart, const Vec2& ptInter,
			Vec2& ptEnd, Vec2& reflectedVectorNormalized);
		void ReflectCircleOnCircle(Vec2& normal, const float interTime, Vec2& velA, const float& massA, Vec2& interPtA,
			Vec2& velB, const float& massB, Vec2& interPtB, Vec2& reflectedVectorA, Vec2& ptEndA, Vec2& reflectedVectorB, Vec2& ptEndB);

		// AXIS-ALIGNED BOUNDING BOX
		void UpdateAABB(AABB& collider, const TransformComponent& transform);
		bool DiscreteAABBvsAABB(const AABB& aabb1, const AABB& aabb2);
		bool ContinuousAABBvsAABB(const AABB& aabb1, const AABB& aabb2, const RigidbodyComponent& body1, const RigidbodyComponent& body2);
		std::array<LineSegment, 4> GenerateEdges(const AABB& aabb) const;

		// ORIENTED BOUNDING BOX (SAT on n-sided polygons)
		void InitOBB(OBB& obb, const std::vector<Vec2>& modelVertices) const;
		void UpdateOBB(OBB& obb, const TransformComponent& trans) const;
		bool DiscreteOBBvsOBB(OBB& lhs, OBB& rhs) const;

		// Wrappers for OBB
		void UpdateVertices(OBB& obb, const TransformComponent& trans) const;
		void UpdateNormals(OBB& obb) const;
		void SATFindMinMax(OBB& obb, const Vec2& currNormal) const;
		inline bool CheckOverlaps(const OBB& lhs, const OBB& rhs) const;
		inline bool IsBetweenBounds(float val, float lowerBound, float upperBound) const;

		// Manifold
		bool InsertDiffPair(Entity a, Entity b);
		bool InsertBoxPair(Entity a, Entity b);
		bool InsertCirclePair(Entity a, Entity b);
		void GenerateDiffManifolds();
		void GenerateBoxManifolds();
		void GenerateCircleManifolds();
		void ResolveManifolds();

		static float GetCorrectionFactor();
		static float GetCorrectionSlop();

		// Collision filters
		bool FilterColliders(const LayerManager::Bits& mask, const LayerManager::Bits& category);
		void AddLayer(std::string_view name);
		void RemoveLayer(size_t layer);
		void RemoveLayer(std::string_view name);
		std::string_view GetLayerName(size_t layer) const;
		void PrintLayerNames() const;
		void PrintCollisionMask(const LayerManager::Bits& mask) const;
		size_t GetNumberOfLayers() const;
		size_t GetLayerCategory(const LayerManager::Bits& cat) const;

		// Collision Events
		bool DetectCollision(const BoxCollider2DComponent& colliderA, const BoxCollider2DComponent& colliderB)
		{
			return DiscreteAABBvsAABB(colliderA.m_aabb, colliderB.m_aabb);
		}

		bool DetectCollision(const BoxCollider2DComponent& colliderA, const CircleCollider2DComponent& colliderB)
		{
			return DiscreteAABBVsCircle(colliderA.m_aabb, colliderB.m_collider);
		}

		bool DetectCollision(const CircleCollider2DComponent& colliderA, const CircleCollider2DComponent colliderB)
		{
			return DiscreteCircleVsCircle(colliderA.m_collider, colliderB.m_collider);
		}

		template <typename TColliderA, typename TColliderB>
		void CheckExitingCollidedPairs(const std::set<Entity>& entities)
		{
			for(auto& pair : m_boxPairs) // TODO: Templatize pair set as well
			{
				// If either entities don't exist in the set anymore, remove.
				if (entities.find(pair.first) == entities.end() || entities.find(pair.second) == entities.end())
				{
					m_eraseQueue.emplace_back(pair);
					continue;
				}

				TColliderA& colliderA = g_engine.m_coordinator.GetComponent<TColliderA>(pair.first);
				TColliderB& colliderB = g_engine.m_coordinator.GetComponent<TColliderB>(pair.second);

				// TODO: Make collision checks templated
				if (!DetectCollision(colliderA, colliderB))
				{
					RigidbodyComponent& bodyA = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(pair.first);
					RigidbodyComponent& bodyB = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(pair.second);
					TransformComponent& transA = g_engine.m_coordinator.GetComponent<TransformComponent>(pair.first);
					TransformComponent& transB = g_engine.m_coordinator.GetComponent<TransformComponent>(pair.second);

					CollisionInfo<TColliderA> infoA(pair.first, colliderA, bodyA, transA);
					CollisionInfo<TColliderB> infoB(pair.second, colliderB, bodyB, transB);

					m_eraseQueue.emplace_back(pair);

					colliderA.SetIsCollided(false);
					colliderB.SetIsCollided(false);
					SendExitEvents(infoA, infoB);
				}
			}

			for (std::pair<Entity, Entity> pair : m_eraseQueue)
			{
				m_boxPairs.erase(pair);
			}

			m_eraseQueue.clear();
		}

		template <typename TColliderA, typename TColliderB>
		void SendEnterEvents(const CollisionInfo<TColliderA>& infoA, const CollisionInfo<TColliderB>& infoB)
		{
			// If A and/or B is/are a trigger(s), dispatch trigger event(s).
			if (infoA.m_collider.GetCollisionMode() == CollisionMode::e_trigger)
			{
				EntTriggerEnterEvent<TColliderA, TColliderB> ev{ infoA, infoB };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
			else if (infoA.m_collider.GetCollisionMode() == CollisionMode::e_awake)
			{
				EntCollisionEnterEvent<TColliderA, TColliderB> ev{ infoA, infoB };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}

			if (infoB.m_collider.GetCollisionMode() == CollisionMode::e_trigger)
			{
				EntTriggerEnterEvent<TColliderB, TColliderA> ev{ infoB, infoA };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
			else if (infoB.m_collider.GetCollisionMode() == CollisionMode::e_awake)
			{
				EntCollisionEnterEvent<TColliderB, TColliderA> ev{ infoB, infoA };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
		}
		
		template <typename TColliderA, typename TColliderB>
		void SendStayEvents(const CollisionInfo<TColliderA>& infoA, const CollisionInfo<TColliderB>& infoB)
		{
			// If A and/or B is/are a trigger(s), dispatch trigger event(s).
			if (infoA.m_collider.GetCollisionMode() == CollisionMode::e_trigger)
			{
				EntTriggerStayEvent<TColliderA, TColliderB> ev{ infoA, infoB };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
			else if (infoA.m_collider.GetCollisionMode() == CollisionMode::e_awake)
			{
				EntCollisionStayEvent<TColliderA, TColliderB> ev{ infoA, infoB };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}

			if (infoB.m_collider.GetCollisionMode() == CollisionMode::e_trigger)
			{
				EntTriggerStayEvent<TColliderB, TColliderA> ev{ infoB, infoA };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
			else if (infoB.m_collider.GetCollisionMode() == CollisionMode::e_awake)
			{
				EntCollisionStayEvent<TColliderB, TColliderA> ev{ infoB, infoA };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
		}

		template <typename TColliderA, typename TColliderB>
		void SendExitEvents(const CollisionInfo<TColliderA>& infoA, const CollisionInfo<TColliderB>& infoB)
		{
			// If A and/or B is/are a trigger(s), dispatch trigger event(s).
			if (infoA.m_collider.GetCollisionMode() == CollisionMode::e_trigger)
			{
				EntTriggerExitEvent<TColliderA, TColliderB> ev{ infoA, infoB };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
			else if (infoA.m_collider.GetCollisionMode() == CollisionMode::e_awake)
			{
				EntCollisionExitEvent<TColliderA, TColliderB> ev{ infoA, infoB };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM); 
				ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}

			if (infoB.m_collider.GetCollisionMode() == CollisionMode::e_trigger)
			{
				EntTriggerExitEvent<TColliderB, TColliderA> ev{ infoB, infoA };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
			else if (infoB.m_collider.GetCollisionMode() == CollisionMode::e_awake)
			{
				EntCollisionExitEvent<TColliderB, TColliderA> ev{ infoB, infoA };
				ev.SetSystemReceivers((int)SystemID::id_LOGICSYSTEM); 
				ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
				EventDispatcher::instance().AddEvent(ev);
			}
		}
	};
}

/*

Broad phase:
------------

Sort and sweep/Spatial partitioning (Most likely don't need)

Iterate through 2 entities

Skip if both are static

Narrow phase:
-------------

In the same loop:
Solve: Dispatch jump table[CirclevsCircle, CirclevsAABB, AABBvsAABB, AABBvsCircle] xxxx

Do discrete tests, and if they collide, insert into collided pairs.

Apply filter tests for collided pairs. If can't collide, then remove.

Generate manifold and emplace_back manifolds.


Iterate through manifolds, and apply impulse and positional correction.

If stacking issue occurs, play around with iterations.
*/