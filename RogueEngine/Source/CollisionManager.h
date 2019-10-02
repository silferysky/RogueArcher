#pragma once
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "AABB.h"
#include "OBB.h"
#include "CircleCollider2DComponent.h"

class CollisionManager
{
	const float HALF_SCALE = 0.5f;
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	// BOUNDING CIRCLE
	int DynamicCircleVsLineSegment(const REMath::Circle& circle,				//Circle data - input 
		const Vec2& ptEnd,			//End circle position - input
		const REMath::LineSegment& lineSeg,			//Line segment - input 
		Vec2& interPt,				//Intersection position of the circle - output 
		Vec2& normalAtCollision,		//Normal vector at collision time - output
		float& interTime,					//Intersection time ti - output
		bool& checkLineEdges);
	int DynamicCircleVsLineEdge(bool withinBothLines,
		const REMath::Circle& circle,
		const Vec2& ptEnd,
		const REMath::LineSegment& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime);
	int CollisionIntersection_RayCircle(const REMath::Ray& ray, const REMath::Circle& circle, float& interTime);
	int CollisionIntersection_CircleCircle(const REMath::Circle& circleA,
		const Vec2& velA,
		const REMath::Circle& circleB,
		const Vec2& velB,
		Vec2& interPtA,
		Vec2& interPtB,
		float& interTime);

	// AXIS-ALIGNED BOUNDING BOX
	void updateAABB(AABB& collider, const TransformComponent& transform);
	bool staticAABBvsAABB(const AABB& aabb1, const AABB& aabb2);
	bool dynamicAABBvsAABB(const AABB& aabb1, const AABB& aabb2, const RigidbodyComponent& body1, const RigidbodyComponent& body2);

	// ORIENTED BOUNDING BOX (SAT on n-sided polygons)
	void initOBB(OBB& obb, const std::vector<Vec2>& modelVertices);
	void updateOBB(OBB& obb, const TransformComponent& trans);
	bool staticOBBvsOBB(OBB& lhs, OBB& rhs);

	// Wrappers for OBB
	void updateVertices(OBB& obb, const TransformComponent& trans);
	void updateNormals(OBB& obb);
	void SATFindMinMax(OBB& obb, const Vec2& currNormal) const;
	inline bool checkOverlaps(const OBB& lhs, const OBB& rhs) const;
	inline bool isBetweenBounds(float val, float lowerBound, float upperBound) const;
};