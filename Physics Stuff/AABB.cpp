#include "Main.h"

Vec2 AABB::getMin() const
{
	return min;
}

Vec2 AABB::getMax() const
{
	return max;
}

void AABB::setMin(const float x, const float y)
{
	min.x = x;
	min.y = y;
}

void AABB::setMax(const float x, const float y)
{
	max.x = x;
	max.y = y;
}

/**************************************************************************/
/* Collision Testing:
Checks for static collision, then checks for moving collision.
Returns a boolean.
*/
/**************************************************************************/
bool AABB::CollisionTest(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2)
{
	if (!staticCollision(rhs))
		return movingCollision(rhs, vel1, vel2);
	else
		return true;
}


/**************************************************************************/
/* Static Collision:
Checks for overlapping of two objects by comparing their boundary boxes.
Returns a boolean.
*/
/**************************************************************************/
inline bool AABB::staticCollision(const AABB &rhs)
{
	// Static collision check
	// Check for min and max values and eliminate false scenarios
	if (max.x < rhs.min.x || min.x > rhs.max.x)
		return false;

	// There's overlap on x. Check for overlap on y.
	if (max.y < rhs.min.y || min.y > rhs.max.y)
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
bool AABB::movingCollision(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2)
{
	// Calculate new relative velocity Vb using vel2
	Vec2 Vb;
	float tFirst = 0;
	float tLast = 0.016f;//deltaTime

	Vb.x = vel2.x - vel1.x;
	Vb.y = vel2.y - vel1.y;

	if (!Vb.x && !Vb.y)
		return false;

	// ===================================
	// Check for collision on the x-axis 
	// ===================================
	if (Vb.x < 0) // Obj2 is moving to the left
	{
		// If obj1 is on the right of obj2
		if (min.x > rhs.max.x)
		{
			// Obj2 is moving away from obj1, no possible collision (CASE 1)
			return false;
		}
		//	If obj1 is on the left of obj2 (CASE 4)
		if (max.x < rhs.min.x)
		{
			// Calculate the time of collision between obj1.x and obj2.x by dividing
			// the distance between the objs by relative velocity
			tFirst = (max.x - rhs.min.x) / Vb.x > tFirst ? (max.x - rhs.min.x) / Vb.x : tFirst;
		}
		if (min.x < rhs.max.x)
		{
			// Calculate the time it takes from now to when obj2.x exits obj1.x
			tLast = (min.x - rhs.max.x) / Vb.x < tLast ? (min.x - rhs.max.x) / Vb.x : tLast;
		}

		// If time taken to enter is longer than time taken to exit (negative time), no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (Vb.x > 0) // Obj2 is moving towards the right
	{
		// Check if obj1 is on the right of obj2 (CASE 2)
		if (min.x > rhs.max.x)
		{
			// Calculate the time of collision for x-axis
			tFirst = (min.x - rhs.max.x) / Vb.x > tFirst ? (min.x - rhs.max.x) / Vb.x : tFirst;
		}
		if (max.x > rhs.min.x)
		{
			// Calculate the time it takes from now to end of collision for x-axis
			tLast = (max.x - rhs.min.x) / Vb.x < tLast ? (max.x - rhs.min.x) / Vb.x : tLast;
		}

		// Check if obj1 is on the left of obj2 (CASE 3)
		if (max.x < rhs.min.x)
		{
			// Obj2 is moving away from obj1, no possible collision
			return false;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (max.x < rhs.min.x || min.x > rhs.max.x)
		return false;

	// ===================================
	// Check for collision on the y-axis	
	// ===================================
	if (Vb.y < 0) // Obj2 is moving to the left
	{
		// If obj1 is on the right of obj2
		if (min.y > rhs.max.y)
		{
			// Obj2 is moving away from obj1, no possible collision (CASE 1)
			return false;
		}
		//	If obj1 is on the left of obj2 (CASE 4)
		if (max.y < rhs.min.y)
		{
			// Calculate the time of collision between obj1.y and obj2.y by dividing
			// the distance between the objs by relative velocity
			tFirst = (max.y - rhs.min.y) / Vb.y > tFirst ? (max.y - rhs.min.y) / Vb.y : tFirst;
		}
		if (min.y < rhs.max.y)
		{
			// Calculate the time it takes from now to when obj2.y exits obj1.y
			tLast = (min.y - rhs.max.y) / Vb.y < tLast ? (min.y - rhs.max.y) / Vb.y : tLast;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}


	else if (Vb.y > 0) // Obj2 is moving towards the right
	{
		// Check if obj1 is on the right of obj2 (CASE 2)
		if (min.y > rhs.max.y)
		{
			// Calculate the time of collision for x-axis
			tFirst = (min.y - rhs.max.y) / Vb.y > tFirst ? (min.y - rhs.max.y) / Vb.y : tFirst;
		}
		if (max.y > rhs.min.y)
		{
			// Calculate the time it takes from now to end of collision for x-axis
			tLast = (max.y - rhs.min.y) / Vb.y < tLast ? (max.y - rhs.min.y) / Vb.y : tLast;
		}

		// Check if obj1 is on the left of obj2 (CASE 3)
		if (max.y < rhs.min.y)
		{
			// Obj2 is moving away from obj1, no possible collision
			return false;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (max.y < rhs.min.y || min.y > rhs.max.y)
		return false;

	// No more possibilities, the rectangles intersect
	return true;
}