#pragma once
#include <vector>

#include "REMath.h"
#include "Vector2D.h"
#include "Matrix33.h"
#include "AABB.h"
#include "OBB.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "CircleCollider2DComponent.h"
#include "Manifold.h"

namespace Rogue
{
	class CollisionManager
	{
		std::vector<std::pair<Entity, Entity>> m_collidedPairs; // Stored during collision tests
		std::vector<Manifold> m_manifolds; // To generate and resolve after collision tests

		static const float s_correction_factor;
		static const float s_correction_slop; // Penetration threshold

		Mtx33 GetColliderWorldMatrix(const BaseCollider& collider, const TransformComponent& trans) const;
		inline Vec2 GetColliderScale(const BaseCollider& collider, const TransformComponent& trans) const;
		inline Vec2 GetColliderPosition(const BaseCollider& collider, const TransformComponent& trans) const;
		inline float GetColliderRotation(const BaseCollider& collider, const TransformComponent& trans) const;

	public:
		static CollisionManager& instance()
		{
			static CollisionManager instance;
			return instance;
		}

		void GenerateManifoldCirclevsCircle(Entity A, Entity B);
		bool GenerateManifoldCirclevsAABB(Entity A, Entity B);
		bool GenerateManifoldAABBvsCircle(Entity A, Entity B);
		void GenerateManifoldAABBvsAABB(Entity A, Entity B);
		void GenerateManifoldAABBvsOBB(Entity A, Entity B);
		void GenerateManifoldOBBvsOBB(Entity A, Entity B);

		CollisionManager() = default;
		~CollisionManager() = default;

		bool DiscreteAABBVsCircle(const AABB& aabb, const CircleCollider& circle);
		bool DiscreteCircleVsAABB(const CircleCollider& circle, const AABB& aabb);

		// BOUNDING CIRCLE
		void UpdateCircleCollider(CircleCollider& circle, const TransformComponent& trans) const;

		bool DiscreteCircleVsCircle(const CircleCollider& circleA, const CircleCollider& circleB,
			const TransformComponent& transA, const TransformComponent& transB);

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
		void InsertColliderPair(Entity a, Entity b);
		void GenerateManifolds(Entity A, Entity B);
		void ResolveManifolds();

		static float GetCorrectionFactor();
		static float GetCorrectionSlop();
	};
}

/*

Broad phase:
------------

Sort and sweep/Spatial partitioning (Most likely don't need)

Iterate through 2 entities

Skip if both are static

Apply filter tests. If can't collide, then skip.

Narrow phase:
-------------

In the same loop:
Solve: Dispatch jump table[CirclevsCircle, CirclevsAABB, AABBvsAABB, AABBvsCircle]

Each function will do discrete tests, and if they collide, generate manifold and emplace_back manifolds.

Iterate through manifolds, and apply impulse and positional correction.

If stacking issue occurs, play around with iterations.
*/