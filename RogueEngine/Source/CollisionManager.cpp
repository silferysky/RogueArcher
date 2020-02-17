/*Start Header*************************************************************************/
/*!
\file           CollisionManager.cpp
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
#include "Precompiled.h"
#include "CollisionManager.h"
#include "Logger.h"
#include "PhysicsDataStructures.hpp"

namespace Rogue
{
	//_________________________________________________________________________
	//_________________________________________________________________________|
	//__________________________STATIC CONSTANTS_______________________________|
	//_________________________________________________________________________|
	//_________________________________________________________________________|
	const float CollisionManager::s_correction_factor = 0.2f;
	const float CollisionManager::s_correction_slop = 0.01f; // Penetration threshold

	//_________________________________________________________________________
	//_________________________________________________________________________|
	//__________________________PRIVATE HELPER FUNCTIONS_______________________|
	//_________________________________________________________________________|
	//_________________________________________________________________________|
	Mtx33 CollisionManager::GetColliderWorldMatrix(const BaseCollider& collider, const TransformComponent& trans) const
	{
		return Mtx33CreateSRTMatrix(GetColliderScale(collider, trans),
									GetColliderRotation(collider, trans),
									GetColliderPosition(collider, trans));
	}

	inline Vec2 CollisionManager::GetColliderScale(const BaseCollider& collider, const TransformComponent& trans) const
	{
		return collider.getScaleOffSet() + Vec2(REAbs(trans.GetScale().x), REAbs(trans.GetScale().y));
	}

	Vec2 CollisionManager::GetColliderPosition(const BaseCollider& collider, const TransformComponent& trans) const
	{
		return collider.getCenterOffSet() + trans.GetPosition();
	}

	inline float CollisionManager::GetColliderRotation(const BaseCollider& collider, const TransformComponent& trans) const
	{
		return collider.getRotationOffSet() + trans.GetRotation();
	}

	void CollisionManager::GenerateManifoldCirclevsCircle(Entity A, Entity B)
	{
		Manifold manifold(A, B);

		auto& transA = g_engine.m_coordinator.GetComponent<TransformComponent>(A);
		auto& transB = g_engine.m_coordinator.GetComponent<TransformComponent>(B);
		auto& circleA = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(A);
		auto& circleB = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(B);
		
		float radiusA = circleA.m_collider.getRadius();
		float radiusB = circleB.m_collider.getRadius();
		Vec2 centerA = transA.GetPosition() + circleA.m_collider.getCenterOffSet();
		Vec2 centerB = transB.GetPosition() + circleB.m_collider.getCenterOffSet();

		Vec2 vAB = centerB - centerA;
		float distAB = Vec2Distance(centerA, centerB); // For colliding circles, actual distance is required.
		float totalRadius = radiusA + radiusB;

		if (distAB == 0.0f)
		{
			manifold.m_penetration = radiusA;
			manifold.m_normal = Vec2::s_unitX; // Can be any direction, but must be consistent
			manifold.m_contactPoints[0] = centerA;
		}
		else
		{
			manifold.m_penetration = totalRadius - distAB;
			manifold.m_normal = vAB / distAB; // Normalized normal.
			manifold.m_contactPoints[0] = manifold.m_normal * radiusA + centerA;
		}

		m_manifolds.emplace_back(manifold);
	}

	bool CollisionManager::GenerateManifoldCirclevsAABB(Entity A, Entity B)
	{
		return GenerateManifoldAABBvsCircle(B, A);
	}

	bool CollisionManager::GenerateManifoldAABBvsCircle(Entity A, Entity B)
	{
		auto& transA = g_engine.m_coordinator.GetComponent<TransformComponent>(A);
		auto& transB = g_engine.m_coordinator.GetComponent<TransformComponent>(B);
		auto& boxA = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(A);
		auto& circleB = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(B);

		Vec2 centerA = GetColliderPosition(boxA.m_aabb, transA);
		Vec2 centerB = GetColliderPosition(circleB.m_collider, transB);
		Vec2 vAB = centerB - centerA;

		// Find the closest point from the AABB's vertex to the center of circle.
		Vec2 closestPt = vAB;

		float AextentX = (boxA.m_aabb.getMax().x - boxA.m_aabb.getMin().x) / 2.0f;
		float AextentY = (boxA.m_aabb.getMax().y - boxA.m_aabb.getMin().y) / 2.0f;

		// Clamp the closest point to the edges of the AABB
		closestPt.x = REClamp(closestPt.x, -AextentX, AextentX);
		closestPt.y = REClamp(closestPt.y, -AextentY, AextentY);

		bool circleIsInside = false;

		// After finding the closestPt, if vAB == closestPt, the center of the circle is inside the AABB.
		if (closestPt == vAB)
		{
			circleIsInside = true;

			// Find closest penetration axis
			if (REAbs(vAB.x) > REAbs(vAB.y)) // x axis is shorter than y axis
			{
				// Clamp the closest point to the closest extent
				closestPt.x = closestPt.x > 0 ?
					AextentX :
					-AextentX;
			}
			else // y penetration is less than x penetration 
			{
				// Clamp to the closest extent
				closestPt.y = closestPt.y > 0 ?
					AextentY :
					-AextentY;
			}
		}

		Vec2 normal = vAB - closestPt;
		float lengthSq = Vec2SqLength(normal);
		float radius = circleB.m_collider.getRadius();

		if (lengthSq > radius * radius && !circleIsInside)
			return false;

		float normalLength = std::sqrtf(lengthSq);

		Manifold manifold(A, B);

		if (circleIsInside)
			manifold.m_normal = -normal / normalLength;
		else
			manifold.m_normal = normal / normalLength;

		manifold.m_penetration = radius - normalLength;

		// Throw into vector of manifolds
		m_manifolds.emplace_back(manifold);

		return true;
	}

	void CollisionManager::GenerateManifoldAABBvsAABB(Entity A, Entity B)
	{
		Manifold manifold(A, B);

		// TODO: Pass components as parameters instead of getting from ECS
		auto& BoxCompA = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(A);
		auto& BoxCompB = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(B);
		auto& TransA = g_engine.m_coordinator.GetComponent<TransformComponent>(A);
		auto& TransB = g_engine.m_coordinator.GetComponent<TransformComponent>(B);

		Vec2 scaleA = TransA.GetScale() + BoxCompA.m_aabb.getScaleOffSet();
		Vec2 scaleB = TransB.GetScale() + BoxCompB.m_aabb.getScaleOffSet();
		Vec2 centerA = TransA.GetPosition() + BoxCompA.m_aabb.getCenterOffSet();
		Vec2 centerB = TransB.GetPosition() + BoxCompB.m_aabb.getCenterOffSet();
		Vec2 vAB = centerB - centerA;

		float AextentX = (BoxCompA.m_aabb.getMax().x - BoxCompA.m_aabb.getMin().x) / 2;
		float BextentX = (BoxCompB.m_aabb.getMax().x - BoxCompB.m_aabb.getMin().x) / 2;
		float AextentY = (BoxCompA.m_aabb.getMax().y - BoxCompA.m_aabb.getMin().y) / 2;
		float BextentY = (BoxCompB.m_aabb.getMax().y - BoxCompB.m_aabb.getMin().y) / 2;

		// ExtentA.x + ExtentB.x - displacement of AB.x
		float x_overlap = AextentX + BextentX - REAbs(vAB.x);

		// ExtentA.y + ExtentB.y - displacement of AB.y
		float y_overlap = AextentY + BextentY - REAbs(vAB.y);

		Vec2 CP1, CP2;
		CP1.x = REMax(BoxCompA.m_aabb.getMin().x, BoxCompB.m_aabb.getMin().x);
		CP1.y = REMax(BoxCompA.m_aabb.getMin().y, BoxCompB.m_aabb.getMin().y);
		CP2.x = REMin(BoxCompA.m_aabb.getMax().x, BoxCompB.m_aabb.getMax().x);
		CP2.y = REMin(BoxCompA.m_aabb.getMax().y, BoxCompB.m_aabb.getMax().y);

		manifold.m_contactPoints[0] = CP1;
		manifold.m_contactPoints[1] = CP2;

		if (x_overlap < y_overlap)
		{
			// Point towards B
			manifold.m_normal = vAB.x > 0 ? Vec2::s_unitX : -Vec2::s_unitX;
			manifold.m_penetration = x_overlap;
		}
		else
		{
			// Point towards A
			manifold.m_normal = vAB.y > 0 ? Vec2::s_unitY : -Vec2::s_unitY;
			manifold.m_penetration = y_overlap;
		}

		m_manifolds.emplace_back(manifold);
	}

	void CollisionManager::GenerateManifoldAABBvsOBB(Entity A, Entity B)
	{
	}

	void CollisionManager::GenerateManifoldOBBvsOBB(Entity A, Entity B)
	{

	}

	bool CollisionManager::DiscreteAABBVsCircle(const AABB& aabb, const BoundingCircle& circle)
	{
		AABB aabb2;
		Vec2 circleCenter = circle.GetCenter();

		aabb2.setMin(Vec2{ circleCenter.x - circle.getRadius(), circleCenter.y - circle.getRadius() });
		aabb2.setMax(Vec2{ circleCenter.x + circle.getRadius(), circleCenter.y + circle.getRadius() });

		return DiscreteAABBvsAABB(aabb, aabb2);
	}

	bool CollisionManager::DiscreteCircleVsAABB(const BoundingCircle& circle, const AABB& aabb)
	{
		return DiscreteAABBVsCircle(aabb, circle);
	}

	bool CollisionManager::DiscretePointVsAABB(const Vec2& point, const AABB& aabb) const
	{
		Vec2 min = aabb.getMin();
		Vec2 max = aabb.getMax();

		if (point.x < min.x || point.x > max.x)
			return false;
		
		if(point.y < min.y || point.y > max.y)
			return false;
		
		return true;
	}

	bool CollisionManager::DiscreteLineVsLine(const LineSegment& lineA, const LineSegment& lineB, Vec2* intersection) const
	{
		Vec2 vecA = lineA.m_pt1 - lineA.m_pt0;
		Vec2 vecB = lineB.m_pt1 - lineB.m_pt0;

		float crossAB = vecA.x * vecB.y - vecA.y * vecB.x;

		if (!crossAB)
			return false;

		Vec2 vecC = lineB.m_pt0 - lineA.m_pt0;
		float t = (vecC.x * vecB.y - vecC.y * vecB.x) / crossAB;

		if (t < 0.0f || t > 1.0f)
			return false;

		float u = (vecC.x * vecA.y - vecC.y * vecA.x) / crossAB;

		if (u < 0.0f || u > 1.0f)
			return false;

		if (intersection)
			*intersection = lineA.m_pt0 + t * vecA;

		return true;
	}

	void CollisionManager::UpdateBoundingCircle(BoundingCircle& circle, const TransformComponent& trans) const
	{
		circle.SetCenter(circle.getCenterOffSet() + trans.GetPosition());
	}

	//_________________________________________________________________________
	//_________________________________________________________________________|
	//__________________________BOUNDING CIRCLE________________________________|
	//_________________________________________________________________________|
	//_________________________________________________________________________|
	bool CollisionManager::DiscreteCircleVsCircle(const BoundingCircle& circleA, const BoundingCircle& circleB)
	{
		float totalRadius = circleA.getRadius() + circleB.getRadius();

		return Vec2SqDistance(circleA.GetCenter(), circleB.GetCenter()) < totalRadius * totalRadius;
	}

	/******************************************************************************/
	/*!
		Check for collision between circle and line.
	 */
	 /******************************************************************************/
	int CollisionManager::ContinuousCircleVsLineSegment(const CircleCollider2DComponent& circle,
		const Vec2& ptEnd,
		const LineSegment& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime,
		bool& checkLineEdges)
	{
		Vec2 P0 = lineSeg.m_pt0;			// One end of the line
		Vec2 P1 = lineSeg.m_pt1;			// Other end of the line
		Vec2 N = lineSeg.m_normal;		// Normal of the line
		Vec2 Bs = circle.m_collider.getCenterOffSet();		// Starting point of the circle
		float R = circle.m_collider.getRadius();				// Radius of the circle


		// If LNS1 is within the range of the radius
		if (Vec2DotProduct(N, Bs - P0) <= -R)
		{
			// Check if velocity vector is within end points of LNS1
			Vec2 P0_ = P0 - R * N; 	// P0', End of imaginary line LNS1
			Vec2 P1_ = P1 - R * N;	// P1', End of imaginary line LNS1
			Vec2 Velocity = ptEnd - Bs; // Velocity vector of circle
			Vec2 M = { Velocity.y, -Velocity.x }; // Outward normal of velocity vector

			if (Vec2DotProduct(M, P0_ - Bs) * Vec2DotProduct(M, P1_ - Bs) < 0.0f)
			{
				interTime = (Vec2DotProduct(N, P0 - Bs) - R) / Vec2DotProduct(N, Velocity); // Displacement divided by velocity gives time.

				if (0.0f <= interTime && interTime <= 1.0f) // If Ti is within 0 and 1, there is collision within this time frame.
				{
					interPt = Bs + interTime * Velocity; // Compute point of intersection
					normalAtCollision = N;
					return 1;
				}
			}
			else if (checkLineEdges)
				return ContinuousCircleVsLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
			else
				return 0;
		}

		// Check for other side of the line.
		else if (Vec2DotProduct(N, Bs - P0) >= R)
		{
			Vec2 P0_ = P0 + R * N;
			Vec2 P1_ = P1 + R * N;
			Vec2 Velocity = ptEnd - Bs;
			Vec2 M = { Velocity.y, -Velocity.x };

			if (Vec2DotProduct(M, P0_ - Bs) * Vec2DotProduct(M, P1_ - Bs) < 0.0f)
			{
				interTime = (Vec2DotProduct(N, P0 - Bs) + R) / Vec2DotProduct(N, Velocity);

				if (0.0f <= interTime && interTime <= 1.0f)
				{
					interPt = Bs + interTime * Velocity;
					normalAtCollision = N;
					return 1; // Collision passed.
				}
			}
			else if (checkLineEdges)
				return ContinuousCircleVsLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
			else
				return 0;
		}

		if (checkLineEdges)
			return ContinuousCircleVsLineEdge(true, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime); // Check for line edges
		else
			return 0;
	}


	/******************************************************************************/
	/*!
		Check for collision between moving circle and the edges of the line segment.
	*/
	/******************************************************************************/
	int CollisionManager::ContinuousCircleVsLineEdge(bool withinBothLines,
		const CircleCollider2DComponent& circle,
		const Vec2& ptEnd,
		const LineSegment& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime)
	{
		if (withinBothLines) // Bs is starting between both imaginary lines.
		{
			// Bs is closer to P0.
			if (Vec2DotProduct(lineSeg.m_pt0 - circle.m_collider.getCenterOffSet(), lineSeg.m_pt1 - lineSeg.m_pt0) > 0)
			{
				Vec2 circleVel = ptEnd - circle.m_collider.getCenterOffSet(); // Velocity vector of the circle.
				Vec2 circleDir; // Normalized directional vector of the circle.
				Vec2Normalize(circleDir, circleVel);
				// Dot product of circle to P0 and circle's vel vector
				float m = Vec2DotProduct(lineSeg.m_pt0 - circle.m_collider.getCenterOffSet(), circleDir);


				if (m > 0) // Circle is facing/moving towards P0
				{
					Vec2 M = { circleDir.y, -circleDir.x }; // Circle's normalized outward normal of it's velocity vector.

					float dist0 = Vec2DotProduct(lineSeg.m_pt0 - circle.m_collider.getCenterOffSet(), M);
					if (abs(dist0) > circle.m_collider.getRadius()) // Circle misses P0. Shortest distance is bigger than the radius
						return 0;

					// Else, circle is moving towards P0.
					float H = sqrt(circle.m_collider.getRadius() * circle.m_collider.getRadius() - dist0 * dist0); // Distance for circle to travel to collide
					interTime = (m - H) / Vec2Length(circleVel);

					if (interTime <= 1)
					{
						interPt = circle.m_collider.getCenterOffSet() + circleVel * interTime;
						Vec2Normalize(normalAtCollision, interPt - lineSeg.m_pt0);
						return 1;
					}
				}
			}
			else // Closer to P1 side
			{
				Vec2 circleVel = ptEnd - circle.m_collider.getCenterOffSet(); // Velocity vector of the circle.
				Vec2 circleDir; // Normalized directional vector of the circle.
				Vec2Normalize(circleDir, circleVel);

				// Dot product of circle to P0 and circle's vel vector
				float m = Vec2DotProduct(lineSeg.m_pt1 - circle.m_collider.getCenterOffSet(), circleDir);

				if (m > 0) // Circle is facing P1
				{
					Vec2 M = { circleDir.y, -circleDir.x }; // Circle's normalized outward normal of it's velocity vector.
					float dist1 = Vec2DotProduct(lineSeg.m_pt1 - circle.m_collider.getCenterOffSet(), M);

					if (abs(dist1) > circle.m_collider.getRadius()) // Circle misses P1. Shortest distance from velocity vector to P1 is greater than radius.
						return 0;

					// Else, circle is moving towards P1.
					float H = sqrt(circle.m_collider.getRadius() * circle.m_collider.getRadius() - dist1 * dist1);
					interTime = (m - H) / Vec2Length(circleVel);

					if (interTime <= 1)
					{
						interPt = circle.m_collider.getCenterOffSet() + circleVel * interTime;
						Vec2Normalize(normalAtCollision, interPt - lineSeg.m_pt1);
						return 1;
					}
				}
			}
		}
		else // Circle is NOT within imaginary lines.
		{
			Vec2 circleVel = ptEnd - circle.m_collider.getCenterOffSet(); // Velocity vector of the circle.
			Vec2 circleDir; // Normalized directional vector of the circle.
			Vec2Normalize(circleDir, circleVel);
			Vec2 M = { circleDir.y, -circleDir.x }; // Circle's normalized outward normal of it's velocity vector.
			bool onP0Side = false;
			float dist0 = Vec2DotProduct(lineSeg.m_pt0 - circle.m_collider.getCenterOffSet(), M);
			float dist1 = Vec2DotProduct(lineSeg.m_pt1 - circle.m_collider.getCenterOffSet(), M);
			float absDist0 = abs(dist0);
			float absDist1 = abs(dist1);

			if (absDist0 > circle.m_collider.getRadius() && absDist1 > circle.m_collider.getRadius())
				return 0; // Circle misses both P0 and P1.

			else if (absDist0 <= circle.m_collider.getRadius() && absDist1 <= circle.m_collider.getRadius())
			{
				float m0 = Vec2DotProduct(lineSeg.m_pt0 - circle.m_collider.getCenterOffSet(), circleDir);
				float m1 = Vec2DotProduct(lineSeg.m_pt1 - circle.m_collider.getCenterOffSet(), circleDir);
				float absM0 = abs(m0);
				float absM1 = abs(m1);

				if (absM0 < absM1)
					onP0Side = true;
				else
					onP0Side = false;
			}

			else if (absDist0 <= circle.m_collider.getRadius())
				onP0Side = true;

			else
				onP0Side = false;

			if (onP0Side) // Circle is closer to P0
			{
				float m = Vec2DotProduct(lineSeg.m_pt0 - circle.m_collider.getCenterOffSet(), circleDir);

				if (m < 0)
					return 0; // Circle is moving away from P0

				// Circle is moving closer to P0
				float H = sqrt(circle.m_collider.getRadius() * circle.m_collider.getRadius() - dist0 * dist0);
				interTime = (m - H) / Vec2Length(circleVel);
				if (interTime <= 1)
				{
					interPt = circle.m_collider.getCenterOffSet() + circleVel * interTime;
					Vec2Normalize(normalAtCollision, interPt - lineSeg.m_pt0);
					return 1;
				}
			}
			else // Circle is closer to P1
			{
				float m = Vec2DotProduct(lineSeg.m_pt1 - circle.m_collider.getCenterOffSet(), circleDir);

				if (m < 0)
					return 0; // Circle is moving away from P1

				// Circle is moving closer to P0
				float H = sqrt(circle.m_collider.getRadius() * circle.m_collider.getRadius() - dist1 * dist1);
				interTime = (m - H) / Vec2Length(circleVel);
				if (interTime <= 1)
				{
					interPt = circle.m_collider.getCenterOffSet() + circleVel * interTime;
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
	int CollisionManager::ContinuousCircleVsRay(const Ray& ray,
		const CircleCollider2DComponent& circle,
		float& interTime)
	{
		Vec2 rayDirNormalized;
		Vec2Normalize(rayDirNormalized, ray.m_dir); // Assign normalized ray directional vector
		float m = Vec2DotProduct(circle.m_collider.getCenterOffSet() - ray.m_pt0, rayDirNormalized); // Distance for the ray to travel to a point closest to the circle

		if (m < 0.0f && Vec2SqDistance(ray.m_pt0, circle.m_collider.getCenterOffSet()) > circle.m_collider.getRadius()* circle.m_collider.getRadius()) // If m < 0 and Bs is outside circle
			return 0; // Circle is behind ray origin.

		float nSquared = Vec2SqDistance(ray.m_pt0, circle.m_collider.getCenterOffSet()) - m * m;

		if (nSquared > circle.m_collider.getRadius()* circle.m_collider.getRadius()) // If n is greater than r
			return 0; // Ray will miss the circle.

		// Compute interTime
		float a = Vec2DotProduct(ray.m_dir, ray.m_dir); // v.v
		float b = -2 * Vec2DotProduct((circle.m_collider.getCenterOffSet() - ray.m_pt0), ray.m_dir); // -2(C-Bs).v
		float c = Vec2DotProduct(circle.m_collider.getCenterOffSet() - ray.m_pt0, circle.m_collider.getCenterOffSet() - ray.m_pt0) - circle.m_collider.getRadius() * circle.m_collider.getRadius(); // (C-Bs).(C-Bs) - r^2
		float det = b * b - (4 * a * c); // b^2 - 4ac

		if (det < RE_EPSILON && det > -RE_EPSILON) // If determinant is 0, ray grazes circle.
		{
			interTime = -b / (2 * a); // Faster calculation (Avoids sqrt)
		}
		else if (det >= RE_EPSILON)
		{
			float s = sqrt(circle.m_collider.getRadius() * circle.m_collider.getRadius() - nSquared);
			float rayLength = Vec2Length(ray.m_dir);
			interTime = REMin((m - s) / rayLength, (m + s) / rayLength);  // (m+-s)/||v||. min(Ti0, Ti1). For pillar, it's always Ti0.
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
	int CollisionManager::ContinuousCircleVsCircle(const CircleCollider2DComponent& circleA,
		const Vec2& velA,
		const CircleCollider2DComponent& circleB,
		const Vec2& velB,
		Vec2& interPtA,
		Vec2& interPtB,
		float& interTime)
	{
		// If second circle is a pillar (Static Circle)
		if (velB.x == 0.0f && velB.y == 0.0f)
		{
			Ray ray; // CircleA is treated as a ray
			CircleCollider2DComponent circle; // Circle is a static pillar
			ray.m_pt0 = circleA.m_collider.getCenterOffSet(); // Assign ray position
			ray.m_dir = velA;
			circle.m_collider.setCenterOffSet(circleB.m_collider.getCenterOffSet()); // Circle has the position of the pillar.
			circle.m_collider.setRadius(circleA.m_collider.getRadius() + circleB.m_collider.getRadius());  // Circle has radius = pillar radius + ray radius.

			if (ContinuousCircleVsRay(ray, circle, interTime))
			{
				interPtA = circleA.m_collider.getCenterOffSet() + interTime * velA; // Bi of ray. interPtB is not needed.
				return 1;
			}
		}
		else
		{
			Ray ray;
			CircleCollider2DComponent circle;

			ray.m_dir = velA - velB;
			ray.m_pt0 = circleA.m_collider.getCenterOffSet();
			circle.m_collider.setCenterOffSet(circleB.m_collider.getCenterOffSet());
			circle.m_collider.setRadius(circleA.m_collider.getRadius() + circleB.m_collider.getRadius());
			if (ContinuousCircleVsRay(ray, circle, interTime))
			{
				interPtA = circleA.m_collider.getCenterOffSet() + interTime * velA;
				interPtB = circleB.m_collider.getCenterOffSet() + interTime * velB;

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
	void CollisionManager::ReflectCircleOnLineSegment(const Vec2& ptInter,
		const Vec2& normal,
		Vec2& ptEnd,
		Vec2& reflected)
	{
		Vec2 i = ptEnd - ptInter;
		ptEnd = ptInter + i - 2.0f * Vec2DotProduct(i, normal) * normal;
		reflected = ptEnd - ptInter;
		Vec2Normalize(reflected, reflected);
	}

	/******************************************************************************/
	/*!
		Reflection computations for moving circle and static circle (Pillar) collisions.
	 */
	 /******************************************************************************/
	void CollisionManager::ReflectCircleOnPillar(const Vec2& normal,
		const float& interTime,
		const Vec2& ptStart,
		const Vec2& ptInter,
		Vec2& ptEnd,
		Vec2& reflectedVectorNormalized)
	{
		(void)ptStart;
		(void)interTime;

		Vec2 i = ptEnd - ptInter;
		ptEnd = ptInter + i - 2.0f * Vec2DotProduct(i, normal) * normal;
		reflectedVectorNormalized = ptEnd - ptInter;
		Vec2Normalize(reflectedVectorNormalized, reflectedVectorNormalized);
	}

	/******************************************************************************/
	/*!
		Reflection computation for moving circle and moving circle.
	 */
	 /******************************************************************************/
	void CollisionManager::ReflectCircleOnCircle(Vec2& normal,
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
		float aA = Vec2DotProduct(velA, normal);
		float aB = Vec2DotProduct(velB, normal);
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
	void CollisionManager::UpdateAABB(AABB& collider, const TransformComponent& transform)
	{
		Vec2 pos = GetColliderPosition(collider, transform);
		Vec2 size = GetColliderScale(collider, transform);
		size.x = REAbs(size.x);
		size.y = REAbs(size.y);
		
		Mtx33 trans, scale;
		
		Mtx33Translate(trans, pos.x, pos.y);
		Mtx33Scale(scale, size.x, size.y);

		trans *= scale;
		Mtx33 result = trans;

		Vec2 min{ -0.5f, -0.5f };
		Vec2 max{ 0.5f, 0.5f };

		collider.setMin(result * min);
		collider.setMax(result * max);
	}


	/**************************************************************************/
	/* Static Collision:
	Checks for overlapping of two objects by comparing their boundary boxes.
	Returns a boolean.
	*/
	/**************************************************************************/
	bool CollisionManager::DiscreteAABBvsAABB(const AABB& aabb1, const AABB& aabb2)
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
	bool CollisionManager::ContinuousAABBvsAABB(const AABB& aabb1, const AABB& aabb2,
		const RigidbodyComponent& body1, const RigidbodyComponent& body2)
	{
		// Calculate new relative velocity Vb using vel2
		float tFirst = 0;
		float tLast = g_deltaTime;

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

	std::array<LineSegment, 4> CollisionManager::GenerateEdges(const AABB& aabb) const
	{
		std::array<LineSegment, 4> edges{ LineSegment(Vec2(aabb.getMin().x, aabb.getMin().y), Vec2(aabb.getMin().x, aabb.getMax().y)),
										LineSegment(Vec2(aabb.getMin().x, aabb.getMax().y), Vec2(aabb.getMax().x, aabb.getMax().y)),
										LineSegment(Vec2(aabb.getMax().x, aabb.getMax().y), Vec2(aabb.getMax().x, aabb.getMin().y)),
										LineSegment(Vec2(aabb.getMax().x, aabb.getMin().y), Vec2(aabb.getMin().x, aabb.getMin().y)) };

		return edges;
	}

	bool CollisionManager::DiscreteLineVsAABB(const LineSegment& line, const AABB& aabb) const
	{
		std::array<LineSegment, 4> edges = GenerateEdges(aabb);

		for (LineSegment edge : edges)
		{
			if (DiscreteLineVsLine(line, edge))
				return true;
		}

		return false;
	}

	bool CollisionManager::DiscreteLineVsAABB(const LineSegment& line, const AABB& aabb, Vec2& intersection) const
	{
		std::array<LineSegment, 4> edges = GenerateEdges(aabb);

		for (LineSegment edge : edges)
		{
			if (!DiscreteLineVsLine(line, edge, &intersection))
				return false;
		}

		return true;
	}


	//_________________________________________________________________________
	//_________________________________________________________________________|
	//_________________________Oriented Bounding Box___________________________|
	//_________________________________________________________________________|
	//_________________________________________________________________________|
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

	void CollisionManager::InitOBB(OBB& obb, const std::vector<Vec2>& modelVertices) const
	{
		obb.modelVerts() = modelVertices;
		obb.globVerts().reserve(modelVertices.size());
		obb.normals().reserve(modelVertices.size());
	}


	void CollisionManager::UpdateOBB(OBB& obb, const TransformComponent& trans) const
	{
		UpdateVertices(obb, trans);
		UpdateNormals(obb);
	}


	void CollisionManager::UpdateVertices(OBB& obb, const TransformComponent& trans) const
	{
		Mtx33 matrix = GetColliderWorldMatrix(obb, trans);

		for (size_t i = 0; i < obb.modelVerts().size(); i++)
		{
			obb.globVerts()[i] = matrix * obb.modelVerts()[i];
		}
	}


	void CollisionManager::UpdateNormals(OBB& obb) const
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
			float result = Vec2DotProduct(vertex, currNormal);

			// We only need to find the extreme mins and maxes of the shape.
			if (result < obb.getMin())
				obb.setMin(result);

			if (result > obb.getMax())
				obb.setMax(result);
		}
	}


	bool CollisionManager::DiscreteOBBvsOBB(OBB& lhs, OBB& rhs) const
	{
		for (Vec2 normal : lhs.normals())
		{
			SATFindMinMax(lhs, normal);
			SATFindMinMax(rhs, normal);

			if (CheckOverlaps(lhs, rhs) == false)
				return false; // No intersection
		}

		for (Vec2 normal : rhs.normals())
		{
			SATFindMinMax(lhs, normal);
			SATFindMinMax(rhs, normal);

			if (CheckOverlaps(rhs, lhs) == false)
				return false; // No intersection
		}

		return true;
	}


	inline bool CollisionManager::CheckOverlaps(const OBB& lhs, const OBB& rhs) const
	{
		return IsBetweenBounds(rhs.getMin(), lhs.getMin(), lhs.getMax()) || IsBetweenBounds(lhs.getMin(), rhs.getMin(), rhs.getMax());
	}

	inline bool CollisionManager::IsBetweenBounds(float val, float lowerBound, float upperBound) const
	{
		return lowerBound <= val && val <= upperBound;
	}

	bool CollisionManager::InsertDiffPair(Entity a, Entity b)
	{
		// Always box first, then circle.
		return m_diffPairs.emplace(std::make_pair(a, b)).second;
	}

	bool CollisionManager::InsertBoxPair(Entity a, Entity b)
	{
		auto pair = m_boxPairs.emplace(std::make_pair(a, b));
		return pair.second;
	}

	bool CollisionManager::InsertCirclePair(Entity a, Entity b)
	{
		return m_circlePairs.emplace(std::make_pair(a, b)).second;
	}

	void CollisionManager::GenerateDiffManifolds()
	{
		for (std::pair<Entity, Entity> pair : m_diffPairs)
		{
			GenerateManifoldAABBvsCircle(pair.first, pair.second);
		}
	}

	void CollisionManager::GenerateBoxManifolds()
	{
		for (std::pair<Entity, Entity> pair : m_boxPairs)
		{
			GenerateManifoldAABBvsAABB(pair.first, pair.second);
		}
	}

	void CollisionManager::GenerateCircleManifolds()
	{	
		for (std::pair<Entity, Entity> pair : m_circlePairs)
		{
			GenerateManifoldCirclevsCircle(pair.first, pair.second);
		}
	}

	void CollisionManager::ResolveManifolds()
	{
		for (auto manifold : m_manifolds)
		{
			manifold.ResolveImpulse();
		//	manifold.ResolveFriction();
			manifold.PositionalCorrection();
		}

		m_manifolds.clear();
	}
	float CollisionManager::GetCorrectionFactor()
	{
		return s_correction_factor;
	}
	float CollisionManager::GetCorrectionSlop()
	{
		return s_correction_slop;
	}

	bool CollisionManager::FilterColliders(const LayerManager::Bits& mask, const LayerManager::Bits& category)
	{
		return LayerManager::instance().FilterLayers(mask, category);
	}

	void CollisionManager::AddLayer(std::string_view name)
	{
		LayerManager::instance().AddLayer(name);
	}

	void CollisionManager::RemoveLayer(size_t layer)
	{
		LayerManager::instance().RemoveLayer(layer);
	}

	void CollisionManager::RemoveLayer(std::string_view name)
	{
		LayerManager::instance().RemoveLayer(name);
	}

	std::string_view CollisionManager::GetLayerName(size_t layer) const
	{
		return LayerManager::instance().GetName(layer);
	}

	void CollisionManager::PrintLayerNames() const
	{
		LayerManager::instance().PrintNames();
	}

	void CollisionManager::PrintCollisionMask(const LayerManager::Bits& mask) const
	{
		LayerManager::instance().PrintMask(mask);
	}

	size_t CollisionManager::GetNumberOfLayers() const
	{
		return LayerManager::instance().GetLayerSize();
	}

	size_t CollisionManager::GetLayerCategory(const LayerManager::Bits& cat) const
	{
		return LayerManager::instance().GetLayerCategory(cat);
	}
}
