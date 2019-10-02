#include "CollisionManager.h"
#include <iostream>

CollisionManager::CollisionManager()
	: HALF_SCALE{ 0.5f }
{}
//_________________________________________________________________________
//_________________________________________________________________________|
//_____________________Axis-Aligned Bounding Box___________________________|
//_________________________________________________________________________|
//_________________________________________________________________________|
// Update the bounding box's m_min and m_max.
void CollisionManager::updateAABB(AABB& collider, const TransformComponent& transform)
{
	collider.setMin(Vec2{ -HALF_SCALE * transform.getScale().x + transform.getPosition().x,
						  -HALF_SCALE * transform.getScale().y + transform.getPosition().y });
	collider.setMax(Vec2{ HALF_SCALE * transform.getScale().x + transform.getPosition().x,
						  HALF_SCALE * transform.getScale().y + transform.getPosition().y });
}


/**************************************************************************/
/* Static Collision:
Checks for overlapping of two objects by comparing their boundary boxes.
Returns a boolean.
*/
/**************************************************************************/
inline bool CollisionManager::staticAABBvsAABB(const AABB& aabb1, const AABB& aabb2)
{
	// Static collision check
	// Check for m_min and m_max values and eliminate false scenarios
	if (aabb1.getMax().x < aabb2.getMin().x || aabb1.getMin().x > aabb2.getMax().x)
		return false;

	// There's overlap on x. Check for overlap on y.
	if (aabb1.getMax().y < aabb2.getMin().y || aabb1.getMin().y > aabb2.getMax().y)
		return false;

	return true;
}


/**************************************************************************/
/*! Moving Collision:
Checks for collision by calculating the time of first contact and last
contact based on relative velocity. If tFirst is less than tLast,
Collision passes.
*/
/**************************************************************************/
bool CollisionManager::dynamicAABBvsAABB(const AABB& aabb1, const AABB& aabb2,
									   const RigidbodyComponent& body1, const RigidbodyComponent& body2)
{
	// Calculate new relative velocity Vb using vel2
	float tFirst = 0;
	float tLast = 0.016f; //deltaTime

	Vec2 Vb(body2.getVelocity() - body1.getVelocity());

	if (!Vb.x && !Vb.y)
		return false;

	// ===================================
	// Check for collision on the x-axis 
	// ===================================
	if (Vb.x < 0) // Obj2 is moving to the left
	{
		// If obj1 is on the right of obj2
		if (aabb1.getMin().x > aabb2.getMax().x)
		{
			// Obj2 is moving away from obj1, no possible collision (CASE 1)
			return false;
		}
		//	If obj1 is on the left of obj2 (CASE 4)
		if (aabb1.getMax().x < aabb2.getMin().x)
		{
			// Calculate the time of collision between obj1.x and obj2.x by dividing
			// the distance between the objs by relative velocity
			tFirst = (aabb1.getMax().x - aabb2.getMin().x) / Vb.x > tFirst ? (aabb1.getMax().x - aabb2.getMin().x) / Vb.x : tFirst;
		}
		if (aabb1.getMin().x < aabb2.getMax().x)
		{
			// Calculate the time it takes from now to when obj2.x exits obj1.x
			tLast = (aabb1.getMin().x - aabb2.getMax().x) / Vb.x < tLast ? (aabb1.getMin().x - aabb2.getMax().x) / Vb.x : tLast;
		}

		// If time taken to enter is longer than time taken to exit (negative time), no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (Vb.x > 0) // Obj2 is moving towards the right
	{
		// Check if obj1 is on the right of obj2 (CASE 2)
		if (aabb1.getMin().x > aabb2.getMax().x)
		{
			// Calculate the time of collision for x-axis
			tFirst = (aabb1.getMin().x - aabb2.getMax().x) / Vb.x > tFirst ? (aabb1.getMin().x - aabb2.getMax().x) / Vb.x : tFirst;
		}
		if (aabb1.getMax().x > aabb2.getMin().x)
		{
			// Calculate the time it takes from now to end of collision for x-axis
			tLast = (aabb1.getMax().x - aabb2.getMin().x) / Vb.x < tLast ? (aabb1.getMax().x - aabb2.getMin().x) / Vb.x : tLast;
		}

		// Check if obj1 is on the left of obj2 (CASE 3)
		if (aabb1.getMax().x < aabb2.getMin().x)
		{
			// Obj2 is moving away from obj1, no possible collision
			return false;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (aabb1.getMax().x < aabb2.getMin().x || aabb1.getMin().x > aabb2.getMax().x)
		return false;

	// ===================================
	// Check for collision on the y-axis	
	// ===================================
	if (Vb.y < 0) // Obj2 is moving to the left
	{
		// If obj1 is on the right of obj2
		if (aabb1.getMin().y > aabb2.getMax().y)
		{
			// Obj2 is moving away from obj1, no possible collision (CASE 1)
			return false;
		}
		//	If obj1 is on the left of obj2 (CASE 4)
		if (aabb1.getMax().y < aabb2.getMin().y)
		{
			// Calculate the time of collision between obj1.y and obj2.y by dividing
			// the distance between the objs by relative velocity
			tFirst = (aabb1.getMax().y - aabb2.getMin().y) / Vb.y > tFirst ? (aabb1.getMax().y - aabb2.getMin().y) / Vb.y : tFirst;
		}
		if (aabb1.getMin().y < aabb2.getMax().y)
		{
			// Calculate the time it takes from now to when obj2.y exits obj1.y
			tLast = (aabb1.getMin().y - aabb2.getMax().y) / Vb.y < tLast ? (aabb1.getMin().y - aabb2.getMax().y) / Vb.y : tLast;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}


	else if (Vb.y > 0) // Obj2 is moving towards the right
	{
		// Check if obj1 is on the right of obj2 (CASE 2)
		if (aabb1.getMin().y > aabb2.getMax().y)
		{
			// Calculate the time of collision for x-axis
			tFirst = (aabb1.getMin().y - aabb2.getMax().y) / Vb.y > tFirst ? (aabb1.getMin().y - aabb2.getMax().y) / Vb.y : tFirst;
		}
		if (aabb1.getMax().y > aabb2.getMin().y)
		{
			// Calculate the time it takes from now to end of collision for x-axis
			tLast = (aabb1.getMax().y - aabb2.getMin().y) / Vb.y < tLast ? (aabb1.getMax().y - aabb2.getMin().y) / Vb.y : tLast;
		}

		// Check if obj1 is on the left of obj2 (CASE 3)
		if (aabb1.getMax().y < aabb2.getMin().y)
		{
			// Obj2 is moving away from obj1, no possible collision
			return false;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (aabb1.getMax().y < aabb2.getMin().y || aabb1.getMin().y > aabb2.getMax().y)
		return false;

	// No more possibilities, the rectangles intersect
	return true;
}

// Debug print
void printOBBGlobs(OBB& obb)
{
	std::vector<Vec2> test(obb.globVerts());

	std::cout << "OBB Globals: " << std::endl;
	for (auto vert : test)
	{
		std::cout << "(" << vert.x << "," << vert.y << ")" << ", ";
	}

	std::cout << std::endl;
}



//_________________________________________________________________________
//_________________________________________________________________________|
//_________________________Oriented Bounding Box___________________________|
//_________________________________________________________________________|
//_________________________________________________________________________|
void CollisionManager::initOBB(OBB& obb, const std::vector<Vec2>& modelVertices)
{
	obb.modelVerts() = modelVertices;
	obb.globVerts().reserve(modelVertices.size());
	obb.normals().reserve(modelVertices.size());
}


void CollisionManager::updateOBB(OBB& obb, const TransformComponent& trans)
{
	updateVertices(obb, trans);
	updateNormals(obb);
}


void CollisionManager::updateVertices(OBB& obb, const TransformComponent& trans)
{
	Mtx33 rot, sca;
	Mtx33RotRad(rot, trans.getRotation());
	Mtx33Scale(sca, trans.getScale().x, trans.getScale().y);
	
	for (int i = 0; i < obb.modelVerts().size(); i++)
	{
		obb.globVerts()[i] = rot * obb.modelVerts()[i];
		obb.globVerts()[i] = sca * obb.modelVerts()[i];
		obb.globVerts()[i] += trans.getPosition();
	}
	}


void CollisionManager::updateNormals(OBB& obb)
{
	size_t i;
	size_t max_sides = obb.modelVerts().size();

	if (max_sides == 0)
		return;

	for (i = 0; i < max_sides - 1; ++i) // Traverse to the second last vertex
		obb.normals()[i] = Vec2NormalOf(obb.globVerts()[i + 1] - obb.globVerts()[i]); // n1 till second last normal

	obb.normals()[i] = Vec2NormalOf(obb.globVerts()[0] - obb.globVerts()[i]); // Last normal
}



void CollisionManager::SATFindMinMax(OBB& obb, const Vec2& currNormal) const
{
	obb.setMin(FLT_MAX); // Initialize as the max float value
	obb.setMax(-obb.getMin());	   // Initialize as the min float value

	for (Vec2 vertex : obb.globVerts()) // Dot every vertex with the current normal
	{
		float result = Vec2DotProd(vertex, currNormal);

		// We only need to find the extreme mins and maxes of the shape.
		if (result < obb.getMin())
			obb.setMin(result);

		if (result > obb.getMax())
			obb.setMax(result);
	}
}


bool CollisionManager::staticOBBvsOBB(OBB& lhs, OBB& rhs)
{
	for (Vec2 normal : lhs.normals())
	{
		SATFindMinMax(lhs, normal);
		SATFindMinMax(rhs, normal);

		if (checkOverlaps(lhs, rhs) == false)
			return false; // No intersection
	}

	for (Vec2 normal : rhs.normals())
	{
		SATFindMinMax(lhs, normal);
		SATFindMinMax(rhs, normal);

		if (checkOverlaps(rhs, lhs) == false)
			return false; // No intersection
	}

	return true;
}


inline bool CollisionManager::checkOverlaps(const OBB& lhs, const OBB& rhs) const
{
	return isBetweenBounds(rhs.getMin(), lhs.getMin(), lhs.getMax()) || isBetweenBounds(lhs.getMin(), rhs.getMin(), rhs.getMax());
}

inline bool CollisionManager::isBetweenBounds(float val, float lowerBound, float upperBound) const
{
	return lowerBound <= val && val <= upperBound;
}