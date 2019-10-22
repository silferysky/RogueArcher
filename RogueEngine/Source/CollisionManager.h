#pragma once
#include "REMath.h"
#include "Manifold.h"
#include "EntityManager.h"
#include <vector>

namespace Rogue
{
	// Forward declarations, for functions that only require interfaces
	class OBB;
	class AABB;
	class BaseCollider;
	class TransformComponent;
	class CircleCollider2DComponent;

	class CollisionManager
	{
		std::vector<std::pair<Entity, Entity>> m_collidedPairs; // Stored during collision tests
		std::vector<Manifold> m_manifolds; // To generate and resolve after collision tests

		Mtx33 GetColliderWorldMatrix(const BaseCollider& collider, const TransformComponent& trans) const;

	public:
		CollisionManager() = default;
		~CollisionManager() = default;

		// BOUNDING CIRCLE
		bool StaticCircleVSCircle(const CircleCollider2DComponent& bc1, const CircleCollider2DComponent& bc2);

		bool DynamicCircleVsLineSegment(const REMath::Circle& circle, const Vec2& ptEnd, const REMath::LineSegment& lineSeg,			
			Vec2& interPt, Vec2& normalAtCollision,	float& interTime, bool& checkLineEdges);

		bool DynamicCircleVsLineEdge(bool withinBothLines, const REMath::Circle& circle, const Vec2& ptEnd,
			const REMath::LineSegment& lineSeg, Vec2& interPt, Vec2& normalAtCollision, float& interTime);

		bool DynamicCircleVSRay(const REMath::Ray& ray, const REMath::Circle& circle, float& interTime);

		bool DynamicCircleVSCircle(const REMath::Circle& circleA, const Vec2& velA, const REMath::Circle& circleB,
			const Vec2& velB, Vec2& interPtA, Vec2& interPtB, float& interTime);

		void CollisionResponse_CircleLineSegment(const Vec2& ptInter, const Vec2& normal, Vec2& ptEnd, Vec2& reflected);

		void CollisionResponse_CirclePillar(const Vec2& normal, const float& interTime, const Vec2& ptStart,
			const Vec2& ptInter, Vec2& ptEnd, Vec2& reflectedVectorNormalized);

		void CollisionResponse_CircleCircle(Vec2& normal, const float interTime, Vec2& velA, const float& massA,
			Vec2& interPtA, Vec2& velB, const float& massB, Vec2& interPtB,
			Vec2& reflectedVectorA, Vec2& ptEndA, Vec2& reflectedVectorB, Vec2& ptEndB);


		// AXIS-ALIGNED BOUNDING BOX
		void updateAABB(AABB& collider, const TransformComponent& trans) const;
		bool staticAABBvsAABB(const AABB& aabb1, const AABB& aabb2);
		bool dynamicAABBvsAABB(const AABB& aabb1, const AABB& aabb2,
			const RigidbodyComponent& body1, const RigidbodyComponent& body2);

		// ORIENTED BOUNDING BOX (SAT on n-sided polygons)
		void initOBB(OBB& obb, const std::vector<Vec2>& modelVertices);
		void updateOBB(OBB& obb, const TransformComponent& trans) const;
		bool staticOBBvsOBB(OBB& lhs, OBB& rhs);

		// Helper functions for OBB
		void updateVertices(OBB& obb, const TransformComponent& trans) const;
		void updateNormals(OBB& obb) const;
		void SATFindMinMax(OBB& obb, const Vec2& currNormal) const;
		inline bool checkOverlaps(const OBB& lhs, const OBB& rhs) const;
		inline bool clamp(float val, float lowerBound, float upperBound) const;

		// Manifold
		void insertColliderPair(Entity a, Entity b);
		void generateManifoldAABBvsAABB(Manifold& m);
		Manifold generateManifold(const OBB& obb1, const OBB& obb2);
	};
}
