#pragma once
#include "Transform.h"
#include "Rigidbody.h"
#include "AABB.h"
#include "OBB.h"
#include "CircleCollider2D.h"

class CollisionManager
{
	const float HALF_SCALE;
public:
	CollisionManager();
	~CollisionManager() = default;

	// AXIS-ALIGNED BOUNDING BOX
	void updateAABB(AABB& collider, const Transform& transform);
	inline bool staticAABBvsAABB(const AABB& aabb1, const AABB& aabb2);
	bool dynamicAABBvsAABB(const AABB& aabb1, const AABB& aabb2, const Rigidbody& body1, const Rigidbody& body2);

	// ORIENTED BOUNDING BOX (SAT on n-sided polygons)
	void initOBB(OBB& obb, const std::vector<Vec2>& modelVertices);
	void updateOBB(OBB& obb, const Transform& trans);
	bool staticOBBvsOBB(OBB& lhs, OBB& rhs);

	// Wrappers for OBB
	void updateVertices(OBB& obb, const Transform& trans);
	void updateNormals(OBB& obb);
	void SATFindMinMax(OBB& obb, const Vec2& currNormal) const;
	inline bool checkOverlaps(const OBB& lhs, const OBB& rhs) const;
	inline bool isBetweenBounds(float val, float lowerBound, float upperBound) const;
};