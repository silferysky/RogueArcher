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

		void GenerateManifoldAABBvsAABB(Manifold& manifold);
		void GenerateManifoldOBBvsOBB(Manifold& manifold);

	public:

		CollisionManager() = default;
		~CollisionManager() = default;

		// BOUNDING CIRCLE
		bool StaticCircleVSCircle(const CircleCollider2DComponent& circleA, const CircleCollider2DComponent& circleB);

		int DynamicCircleVsLineSegment(const CircleCollider2DComponent& circle, const Vec2& ptEnd, const LineSegment& lineSeg,	
			Vec2& interPt, Vec2& normalAtCollision, float& interTime, bool& checkLineEdges);

		int DynamicCircleVsLineEdge(bool withinBothLines, const CircleCollider2DComponent& circle, const Vec2& ptEnd,
			const LineSegment& lineSeg, Vec2& interPt, Vec2& normalAtCollision, float& interTime);

		int CollisionIntersection_RayCircle(const Ray& ray, const CircleCollider2DComponent& circle, float& interTime);
		int CollisionIntersection_CircleCircle(const CircleCollider2DComponent& circleA, const Vec2& velA,
			const CircleCollider2DComponent& circleB, const Vec2& velB, Vec2& interPtA, Vec2& interPtB, float& interTime);

		void CollisionResponse_CircleLineSegment(const Vec2& ptInter, const Vec2& normal, Vec2& ptEnd, Vec2& reflected);

		void CollisionResponse_CirclePillar(const Vec2& normal, const float& interTime, const Vec2& ptStart, const Vec2& ptInter,
			Vec2& ptEnd, Vec2& reflectedVectorNormalized);

		void CollisionResponse_CircleCircle(Vec2& normal, const float interTime, Vec2& velA, const float& massA, Vec2& interPtA,
			Vec2& velB, const float& massB, Vec2& interPtB, Vec2& reflectedVectorA, Vec2& ptEndA, Vec2& reflectedVectorB, Vec2& ptEndB);

		// AXIS-ALIGNED BOUNDING BOX
		void updateAABB(AABB& collider, const TransformComponent& transform);
		bool staticAABBvsAABB(const AABB& aabb1, const AABB& aabb2);
		bool dynamicAABBvsAABB(const AABB& aabb1, const AABB& aabb2, const RigidbodyComponent& body1, const RigidbodyComponent& body2);

		// ORIENTED BOUNDING BOX (SAT on n-sided polygons)
		void initOBB(OBB& obb, const std::vector<Vec2>& modelVertices) const;
		void updateOBB(OBB& obb, const TransformComponent& trans) const;
		bool staticOBBvsOBB(OBB& lhs, OBB& rhs) const;

		// Wrappers for OBB
		void updateVertices(OBB& obb, const TransformComponent& trans) const;
		void updateNormals(OBB& obb) const;
		void SATFindMinMax(OBB& obb, const Vec2& currNormal) const;
		inline bool checkOverlaps(const OBB& lhs, const OBB& rhs) const;
		inline bool isBetweenBounds(float val, float lowerBound, float upperBound) const;

		// Manifold
		void InsertColliderPair(Entity a, Entity b);
		void GenerateManifolds();
		void ResolveManifolds();

		static float GetCorrectionFactor();
		static float GetCorrectionSlop();
	};
}
