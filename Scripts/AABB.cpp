#include "Physics.h"

Vec2 AABB::getMin() const
{
	return m_min;
}

Vec2 AABB::getMax() const
{
	return m_max;
}

void AABB::setMin(float x, float y)
{
	m_min.x = x;
	m_min.y = y;
}

void AABB::setMax(float x, float y)
{
	m_max.x = x;
	m_max.y = y;
}

// Update the bounding box's m_min and m_max.
inline void AABB::update(const Vec2& pos, const float scale)
{
	setMin(-0.5f * scale + pos.x, -0.5f * scale + pos.y);
	setMax(0.5f * scale + pos.x, 0.5f * scale + pos.y);
}

/**************************************************************************/
/* Collision Testing:
Checks for static collision, then checks for moving collision.
Returns a boolean.
*/
/**************************************************************************/
bool AABB::collisionTest(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2)
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
	// Check for m_min and m_max values and elim_minate false scenarios
	if (m_max.x < rhs.m_min.x || m_min.x > rhs.m_max.x)
		return false;

	// There's overlap on x. Check for overlap on y.
	if (m_max.y < rhs.m_min.y || m_min.y > rhs.m_max.y)
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
	float tLast = 0.016f; //deltaTime

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
		if (m_min.x > rhs.m_max.x)
		{
			// Obj2 is moving away from obj1, no possible collision (CASE 1)
			return false;
		}
		//	If obj1 is on the left of obj2 (CASE 4)
		if (m_max.x < rhs.m_min.x)
		{
			// Calculate the time of collision between obj1.x and obj2.x by dividing
			// the distance between the objs by relative velocity
			tFirst = (m_max.x - rhs.m_min.x) / Vb.x > tFirst ? (m_max.x - rhs.m_min.x) / Vb.x : tFirst;
		}
		if (m_min.x < rhs.m_max.x)
		{
			// Calculate the time it takes from now to when obj2.x exits obj1.x
			tLast = (m_min.x - rhs.m_max.x) / Vb.x < tLast ? (m_min.x - rhs.m_max.x) / Vb.x : tLast;
		}

		// If time taken to enter is longer than time taken to exit (negative time), no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (Vb.x > 0) // Obj2 is moving towards the right
	{
		// Check if obj1 is on the right of obj2 (CASE 2)
		if (m_min.x > rhs.m_max.x)
		{
			// Calculate the time of collision for x-axis
			tFirst = (m_min.x - rhs.m_max.x) / Vb.x > tFirst ? (m_min.x - rhs.m_max.x) / Vb.x : tFirst;
		}
		if (m_max.x > rhs.m_min.x)
		{
			// Calculate the time it takes from now to end of collision for x-axis
			tLast = (m_max.x - rhs.m_min.x) / Vb.x < tLast ? (m_max.x - rhs.m_min.x) / Vb.x : tLast;
		}

		// Check if obj1 is on the left of obj2 (CASE 3)
		if (m_max.x < rhs.m_min.x)
		{
			// Obj2 is moving away from obj1, no possible collision
			return false;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (m_max.x < rhs.m_min.x || m_min.x > rhs.m_max.x)
		return false;

	// ===================================
	// Check for collision on the y-axis	
	// ===================================
	if (Vb.y < 0) // Obj2 is moving to the left
	{
		// If obj1 is on the right of obj2
		if (m_min.y > rhs.m_max.y)
		{
			// Obj2 is moving away from obj1, no possible collision (CASE 1)
			return false;
		}
		//	If obj1 is on the left of obj2 (CASE 4)
		if (m_max.y < rhs.m_min.y)
		{
			// Calculate the time of collision between obj1.y and obj2.y by dividing
			// the distance between the objs by relative velocity
			tFirst = (m_max.y - rhs.m_min.y) / Vb.y > tFirst ? (m_max.y - rhs.m_min.y) / Vb.y : tFirst;
		}
		if (m_min.y < rhs.m_max.y)
		{
			// Calculate the time it takes from now to when obj2.y exits obj1.y
			tLast = (m_min.y - rhs.m_max.y) / Vb.y < tLast ? (m_min.y - rhs.m_max.y) / Vb.y : tLast;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}


	else if (Vb.y > 0) // Obj2 is moving towards the right
	{
		// Check if obj1 is on the right of obj2 (CASE 2)
		if (m_min.y > rhs.m_max.y)
		{
			// Calculate the time of collision for x-axis
			tFirst = (m_min.y - rhs.m_max.y) / Vb.y > tFirst ? (m_min.y - rhs.m_max.y) / Vb.y : tFirst;
		}
		if (m_max.y > rhs.m_min.y)
		{
			// Calculate the time it takes from now to end of collision for x-axis
			tLast = (m_max.y - rhs.m_min.y) / Vb.y < tLast ? (m_max.y - rhs.m_min.y) / Vb.y : tLast;
		}

		// Check if obj1 is on the left of obj2 (CASE 3)
		if (m_max.y < rhs.m_min.y)
		{
			// Obj2 is moving away from obj1, no possible collision
			return false;
		}

		// If time taken to enter is longer than time taken to exit, no collision possible
		if (tFirst > tLast)
			return false;
	}

	else if (m_max.y < rhs.m_min.y || m_min.y > rhs.m_max.y)
		return false;

	// No more possibilities, the rectangles intersect
	return true;
}