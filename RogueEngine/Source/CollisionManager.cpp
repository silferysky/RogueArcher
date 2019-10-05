#include "CollisionManager.h"
#include <iostream>
#include "Main.h"

//_________________________________________________________________________
//_________________________________________________________________________|
//__________________________BOUNDING CIRCLE________________________________|
//_________________________________________________________________________|
//_________________________________________________________________________|
/******************************************************************************/
/*!
	Check for collision between circle and line.
 */
 /******************************************************************************/
int CollisionManager::DynamicCircleVsLineSegment(const REMath::Circle& circle,				//Circle data - input 
	const Vec2& ptEnd,			//End circle position - input
	const REMath::LineSegment& lineSeg,			//Line segment - input 
	Vec2& interPt,				//Intersection position of the circle - output 
	Vec2& normalAtCollision,		//Normal vector at collision time - output
	float& interTime,					//Intersection time ti - output
	bool& checkLineEdges)				//true = check collision with line segment edges
{
	Vec2 P0 = lineSeg.m_pt0;			// One end of the line
	Vec2 P1 = lineSeg.m_pt1;			// Other end of the line
	Vec2 N = lineSeg.m_normal;		// Normal of the line
	Vec2 Bs = circle.m_center;		// Starting point of the circle
	float R = circle.m_radius;				// Radius of the circle


	// If LNS1 is within the range of the radius
	if (Vec2DotProd(N, Bs - P0) <= -R)
	{
		// Check if velocity vector is within end points of LNS1
		Vec2 P0_ = P0 - R * N; 	// P0', End of imaginary line LNS1
		Vec2 P1_ = P1 - R * N;	// P1', End of imaginary line LNS1
		Vec2 Velocity = ptEnd - Bs; // Velocity vector of circle
		Vec2 M = { Velocity.y, -Velocity.x }; // Outward normal of velocity vector

		if (Vec2DotProd(M, P0_ - Bs) * Vec2DotProd(M, P1_ - Bs) < 0.0f)
		{
			interTime = (Vec2DotProd(N, P0 - Bs) - R) / Vec2DotProd(N, Velocity); // Displacement divided by velocity gives time.

			if (0.0f <= interTime && interTime <= 1.0f) // If Ti is within 0 and 1, there is collision within this time frame.
			{
				interPt = Bs + interTime * Velocity; // Compute point of intersection
				normalAtCollision = N;
				return 1;
			}
		}
		else if (checkLineEdges)
			return DynamicCircleVsLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
		else
			return 0;
	}

	// Check for other side of the line.
	else if (Vec2DotProd(N, Bs - P0) >= R)
	{
		Vec2 P0_ = P0 + R * N;
		Vec2 P1_ = P1 + R * N;
		Vec2 Velocity = ptEnd - Bs;
		Vec2 M = { Velocity.y, -Velocity.x };

		if (Vec2DotProd(M, P0_ - Bs) * Vec2DotProd(M, P1_ - Bs) < 0.0f)
		{
			interTime = (Vec2DotProd(N, P0 - Bs) + R) / Vec2DotProd(N, Velocity);

			if (0.0f <= interTime && interTime <= 1.0f)
			{
				interPt = Bs + interTime * Velocity;
				normalAtCollision = N;
				return 1; // Collision passed.
			}
		}
		else if (checkLineEdges)
			return DynamicCircleVsLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
		else
			return 0;
	}

	if (checkLineEdges)
		return DynamicCircleVsLineEdge(true, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime); // Check for line edges
	else
		return 0;
}


/******************************************************************************/
/*!
	Check for collision between moving circle and the edges of the line segment.
*/
/******************************************************************************/
int CollisionManager::DynamicCircleVsLineEdge(bool withinBothLines,
	const REMath::Circle& circle,
	const Vec2& ptEnd,
	const REMath::LineSegment& lineSeg,
	Vec2& interPt,
	Vec2& normalAtCollision,
	float& interTime)
{
	if (withinBothLines) // Bs is starting between both imaginary lines.
	{
		// Bs is closer to P0.
		if (Vec2DotProd(lineSeg.m_pt0 - circle.m_center, lineSeg.m_pt1 - lineSeg.m_pt0) > 0)
		{
			Vec2 circleVel = ptEnd - circle.m_center; // Velocity vector of the circle.
			Vec2 circleDir; // Normalized directional vector of the circle.
			Vec2Normalize(circleDir, circleVel);
			// Dot product of circle to P0 and circle's vel vector
			float m = Vec2DotProd(lineSeg.m_pt0 - circle.m_center, circleDir);


			if (m > 0) // Circle is facing/moving towards P0
			{
				Vec2 M = { circleDir.y, -circleDir.x }; // Circle's normalized outward normal of it's velocity vector.

				float dist0 = Vec2DotProd(lineSeg.m_pt0 - circle.m_center, M);
				if (abs(dist0) > circle.m_radius) // Circle misses P0. Shortest distance is bigger than the radius
					return 0;

				// Else, circle is moving towards P0.
				float H = sqrt(circle.m_radius * circle.m_radius - dist0 * dist0); // Distance for circle to travel to collide
				interTime = (m - H) / Vec2Length(circleVel);

				if (interTime <= 1)
				{
					interPt = circle.m_center + circleVel * interTime;
					Vec2Normalize(normalAtCollision, interPt - lineSeg.m_pt0);
					return 1;
				}
			}
		}
		else // Closer to P1 side
		{
			Vec2 circleVel = ptEnd - circle.m_center; // Velocity vector of the circle.
			Vec2 circleDir; // Normalized directional vector of the circle.
			Vec2Normalize(circleDir, circleVel);

			// Dot product of circle to P0 and circle's vel vector
			float m = Vec2DotProd(lineSeg.m_pt1 - circle.m_center, circleDir);

			if (m > 0) // Circle is facing P1
			{
				Vec2 M = { circleDir.y, -circleDir.x }; // Circle's normalized outward normal of it's velocity vector.
				float dist1 = Vec2DotProd(lineSeg.m_pt1 - circle.m_center, M);

				if (abs(dist1) > circle.m_radius) // Circle misses P1. Shortest distance from velocity vector to P1 is greater than radius.
					return 0;

				// Else, circle is moving towards P1.
				float H = sqrt(circle.m_radius * circle.m_radius - dist1 * dist1);
				interTime = (m - H) / Vec2Length(circleVel);

				if (interTime <= 1)
				{
					interPt = circle.m_center + circleVel * interTime;
					Vec2Normalize(normalAtCollision, interPt - lineSeg.m_pt1);
					return 1;
				}
			}
		}
	}
	else // Circle is NOT within imaginary lines.
	{
		Vec2 circleVel = ptEnd - circle.m_center; // Velocity vector of the circle.
		Vec2 circleDir; // Normalized directional vector of the circle.
		Vec2Normalize(circleDir, circleVel);
		Vec2 M = { circleDir.y, -circleDir.x }; // Circle's normalized outward normal of it's velocity vector.
		bool onP0Side = false;
		float dist0 = Vec2DotProd(lineSeg.m_pt0 - circle.m_center, M);
		float dist1 = Vec2DotProd(lineSeg.m_pt1 - circle.m_center, M);
		float absDist0 = abs(dist0);
		float absDist1 = abs(dist1);

		if (absDist0 > circle.m_radius&& absDist1 > circle.m_radius)
			return 0; // Circle misses both P0 and P1.

		else if (absDist0 <= circle.m_radius && absDist1 <= circle.m_radius)
		{
			float m0 = Vec2DotProd(lineSeg.m_pt0 - circle.m_center, circleDir);
			float m1 = Vec2DotProd(lineSeg.m_pt1 - circle.m_center, circleDir);
			float absM0 = abs(m0);
			float absM1 = abs(m1);

			if (absM0 < absM1)
				onP0Side = true;
			else
				onP0Side = false;
		}

		else if (absDist0 <= circle.m_radius)
			onP0Side = true;

		else
			onP0Side = false;

		if (onP0Side) // Circle is closer to P0
		{
			float m = Vec2DotProd(lineSeg.m_pt0 - circle.m_center, circleDir);

			if (m < 0)
				return 0; // Circle is moving away from P0

			// Circle is moving closer to P0
			float H = sqrt(circle.m_radius * circle.m_radius - dist0 * dist0);
			interTime = (m - H) / Vec2Length(circleVel);
			if (interTime <= 1)
			{
				interPt = circle.m_center + circleVel * interTime;
				Vec2Normalize(normalAtCollision, interPt - lineSeg.m_pt0);
				return 1;
			}
		}
		else // Circle is closer to P1
		{
			float m = Vec2DotProd(lineSeg.m_pt1 - circle.m_center, circleDir);

			if (m < 0)
				return 0; // Circle is moving away from P1

			// Circle is moving closer to P0
			float H = sqrt(circle.m_radius * circle.m_radius - dist1 * dist1);
			interTime = (m - H) / Vec2Length(circleVel);
			if (interTime <= 1)
			{
				interPt = circle.m_center + circleVel * interTime;
				Vec2Normalize(normalAtCollision, interPt - lineSeg.m_pt1);
				return 1;
			}
		}
	}

	return 0; //no collision
}
/******************************************************************************/
/*!
	Check for collision between moving circle and static circle (Pillar).
 */
 /******************************************************************************/
int CollisionManager::CollisionIntersection_RayCircle(const REMath::Ray& ray,
	const REMath::Circle& circle,
	float& interTime)
{
	Vec2 rayDirNormalized;
	Vec2Normalize(rayDirNormalized, ray.m_dir); // Assign normalized ray directional vector
	float m = Vec2DotProd(circle.m_center - ray.m_pt0, rayDirNormalized); // Distance for the ray to travel to a point closest to the circle

	if (m < 0.0f && Vec2SqDistance(ray.m_pt0, circle.m_center) > circle.m_radius* circle.m_radius) // If m < 0 and Bs is outside circle
		return 0; // Circle is behind ray origin.

	float nSquared = Vec2SqDistance(ray.m_pt0, circle.m_center) - m * m;

	if (nSquared > circle.m_radius* circle.m_radius) // If n is greater than r
		return 0; // Ray will miss the circle.

	// Compute interTime
	float a = Vec2DotProd(ray.m_dir, ray.m_dir); // v.v
	float b = -2 * Vec2DotProd((circle.m_center - ray.m_pt0), ray.m_dir); // -2(C-Bs).v
	float c = Vec2DotProd(circle.m_center - ray.m_pt0, circle.m_center - ray.m_pt0) - circle.m_radius * circle.m_radius; // (C-Bs).(C-Bs) - r^2
	float det = b * b - (4 * a * c); // b^2 - 4ac

	if (det < REMath::EPSILON && det > -REMath::EPSILON) // If determinant is 0, ray grazes circle.
	{
		interTime = -b / (2 * a); // Faster calculation (Avoids sqrt)
	}
	else if (det >= REMath::EPSILON)
	{
		float s = sqrt(circle.m_radius * circle.m_radius - nSquared);
		float rayLength = Vec2Length(ray.m_dir);
		interTime = std::min((m - s) / rayLength, (m + s) / rayLength);  // (m+-s)/||v||. min(Ti0, Ti1). For pillar, it's always Ti0.
	}
	if (interTime > 0.0f && interTime < 1.0f) // If time of intersection is between 0 and 1
		return 1; // Ray intersects static circle (Pillar).

	// Else, no collision.
	return 0;
}

/******************************************************************************/
/*!
	Check for collision between moving circle and static/dynamic circle.
 */
 /******************************************************************************/
int CollisionManager::CollisionIntersection_CircleCircle(const REMath::Circle& circleA,
	const Vec2& velA,
	const REMath::Circle& circleB,
	const Vec2& velB,
	Vec2& interPtA,
	Vec2& interPtB,
	float& interTime)
{
	// If second circle is a pillar (Static Circle)
	if (velB.x == 0.0f && velB.y == 0.0f)
	{
		REMath::Ray ray; // CircleA is treated as a ray
		REMath::Circle circle; // Circle is a static pillar
		ray.m_pt0 = circleA.m_center; // Assign ray position
		ray.m_dir = velA;
		circle.m_center = circleB.m_center; // Circle has the position of the pillar.
		circle.m_radius = circleA.m_radius + circleB.m_radius;  // Circle has radius = pillar radius + ray radius.

		if (CollisionIntersection_RayCircle(ray, circle, interTime))
		{
			interPtA = circleA.m_center + interTime * velA; // Bi of ray. interPtB is not needed.
			return 1;
		}
	}
	else
	{
		REMath::Ray ray;
		REMath::Circle circle;

		ray.m_dir = velA - velB;
		ray.m_pt0 = circleA.m_center;
		circle.m_center = circleB.m_center;
		circle.m_radius = circleA.m_radius + circleB.m_radius;
		if (CollisionIntersection_RayCircle(ray, circle, interTime))
		{
			interPtA = circleA.m_center + interTime * velA;
			interPtB = circleB.m_center + interTime * velB;

			return 1;
		}
	}

	return 0;
}



/******************************************************************************/
/*!
	Reflection computations for circle and line segment collisions.
 */
 /******************************************************************************/
void CollisionManager::CollisionResponse_CircleLineSegment(const Vec2& ptInter,
	const Vec2& normal,
	Vec2& ptEnd,
	Vec2& reflected)
{
	Vec2 i = ptEnd - ptInter;
	ptEnd = ptInter + i - 2.0f * Vec2DotProd(i, normal) * normal;
	reflected = ptEnd - ptInter;
	Vec2Normalize(reflected, reflected);
}

/******************************************************************************/
/*!
	Reflection computations for moving circle and static circle (Pillar) collisions.
 */
 /******************************************************************************/
void CollisionManager::CollisionResponse_CirclePillar(const Vec2& normal,
	const float& interTime,
	const Vec2& ptStart,
	const Vec2& ptInter,
	Vec2& ptEnd,
	Vec2& reflectedVectorNormalized)
{
	(void)ptStart;
	(void)interTime;

	Vec2 i = ptEnd - ptInter;
	ptEnd = ptInter + i - 2.0f * Vec2DotProd(i, normal) * normal;
	reflectedVectorNormalized = ptEnd - ptInter;
	Vec2Normalize(reflectedVectorNormalized, reflectedVectorNormalized);
}

/******************************************************************************/
/*!
	Extra credits: Reflection computation for moving circle and moving circle.
 */
 /******************************************************************************/
void CollisionManager::CollisionResponse_CircleCircle(Vec2& normal,
	const float interTime,
	Vec2& velA,
	const float& massA,
	Vec2& interPtA,
	Vec2& velB,
	const float& massB,
	Vec2& interPtB,
	Vec2& reflectedVectorA,
	Vec2& ptEndA,
	Vec2& reflectedVectorB,
	Vec2& ptEndB)
{
	float aA = Vec2DotProd(velA, normal);
	float aB = Vec2DotProd(velB, normal);
	reflectedVectorA = velA - 2 * (aA - aB) / (massA + massB) * massB * normal;
	reflectedVectorB = velB + 2 * (aA - aB) / (massA + massB) * massA * normal;
	ptEndA = interPtA + (1 - interTime) * reflectedVectorA;
	ptEndB = interPtB + (1 - interTime) * reflectedVectorB;
}
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
bool CollisionManager::staticAABBvsAABB(const AABB& aabb1, const AABB& aabb2)
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
	float tLast = gDeltaTime;

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
	
	for (int i = 0; i < (int)obb.modelVerts().size(); i++)
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

	if (!max_sides)
		RE_CORE_ERROR("OBB has no sides!");
	
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
	//std::vector<Vec2>::iterator i;
	//for(i = lhs.globVerts().begin(); i != lhs.globVerts().cend(); ++i)
	//	std::cout << *i << std::endl;

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