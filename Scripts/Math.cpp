#include "Math.h"
#include <algorithm>

namespace Math
{
	const double PI = 3.1415926535897;

	class Math
	{
		double Math::DegreesToRadians(double degrees) const
		{
			return (degrees * PI / 180.0);
		}

		double Math::RadiansToDegrees(double radians) const
		{
			return (radians * 180.0 / PI);
		}

		float Math::NewSin(float x)
		{
			float result = sin(x);
			return result;
		}

		float Math::NewCos(float x)
		{
			float result = cos(x);
			return result;
		}

		float Math::NewTan(float x)
		{
			float result = tan(x);
			return result;
		}

		float Math::NewASin(float x)
		{
			float result = asin(x);
			return result;
		}

		float Math::NewACos(float x)
		{
			float result = acos(x);
			return result;
		}

		float Math::NewATan(float x)
		{
			float result = atan(x);
			return result;
		}

		double Math::NewLogBase2(double x)
		{
			double result = log2(x);
			return result;
		}

		float Math::NewClamp(float x, float x0, float x1)
		{
			if (x < x0)
			{
				return x0;
			}
			else if (x > x1)
			{
				return x1;
			}
			else
			{
				return x;
			}
		}

		float Math::NewWrap(float x, float x0, float x1)
		{
			float range = x1 - x0;
			if (x < (x0 - range) || (x > (x1 + range))
			{
				return x;
			}

			else
			{
				if (x < x0)
				{
					return (x + range);
				}
				if (x > x1)
				{
					return (x - range);
				}
			}
		}

		float Math::NewMin(float x, float y)
		{
			if (x < y)
			{
				return x;
			}
			else
			{
				return y;
			}
		}

		float Math::Math::NewMax(float x, float y)
		{
			if (x > y)
			{
				return x;
			}
			else
			{
				return y;
			}
		}

		bool Math::Math::NewInRange(float x, float x0, float x1)
		{
			if (x >= x0 || x <= x1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		float Math::Math::NewDistPointToCircle(Vector::Vec2 * pPos, Vector::Vec2 * pCenter, float radius)
		{
			float result;
			float x1 = (pPos->x - pCenter->x) * (pPos->x - pCenter->x);
			float y1 = (pPos->y - pCenter->y) * (pPos->y - pCenter->y);
			result = sqrt(x1 + y1) - radius;
			return result;
		}

		float Math::Math::NewDistPointToRectangle(Vector::Vec2 * pPos, Vector::Vec2 * pRect, float width, float height)
		{
			float dx, dy;
			dx = max(abs(pPos->x - pRect->x) - width / 2, 0);
			dy = max(abs(pPos->x - pRect->x) - height / 2, 0);
			return (dx * dx + dy * dy);
		}

		float Math::Math::NewDistPointToLineSegment(Vector::Vec2 * pPos, Vector::Vec2 pLine0, Vector::Vec2 pLine1)
		{
			float distance1 = pLine1.x - pLine0.x;
			float distance2 = pLine1.y - pLine0.y;
			float distance3 = pLine1.x * pLine0.y - pLine1.y * pLine0.x;

			return abs(distance1 * pPos.x + distance2 * pPos.y + distance3) / (distance1 * distance1 + distance2 * distance2);
		}

		float Math::Math::NewDistCircleToCircle(Vector::Vec2 * pCenter0, Vector::Vec2 * pCenter1, float radius0, float radius1)
		{
			float distance = Vector::Vector2DDistance(pCenter0, pCenter1) - (radius0 + radius1);
			return distance;
		}

		float Math::Math::NewDistCircleToRectangle(Vector::Vec2 * pCenter, Vector::Vec2 * pRect, float radius, float SizeX, float SizeY)
		{
			float DeltaX, DeltaY;
			DeltaX = pCenter->x - Max(pRect->x, Min(pCenter->x, pRect->x + SizeX));
			DeltaY = pCenter->y - Max(pRect->y, Min(pCenter->y, pRect->y + SizeY));
			return (DeltaX * DeltaX + DeltaY * DeltaY) < (radius * radius);
		}

		float Math::Math::NewDistRectangleToRectangle(Vector::Vec2 * pRect0, float SizeX0, float SizeY0, Vector::Vec2 * pRect1, float SizeX1, float SizeY1)
		{	 
			return max(abs(pRect1->x - pRect0->x) - (SizeX0 + SizeX1) / 2, abs(pRect1->y - pRect0->y) - (SizeY1 + SizeY0) / 2);
		}

		//bool Math::Math::NewTestPointToCircle(Vector::Vec2 * pPos, Vector::Vec2 * pCenter, float radius)
		//{
		//	return false;
		//}
		//
		//bool Math::Math::NewTestPointToRectangle(Vector::Vec2 * pPos, Vector::Vec2 * pRect, float SizeX, float SizeY)
		//{
		//	return false;
		//}
		//
		//bool Math::Math::NewTestCircleToCircle(Vector::Vec2 * pCenter0, Vector::Vec2 * pCenter1, float radius0, float radius1)
		//{
		//	return false;
		//}
		//
		//bool Math::Math::NewTestCircleToRectangle(Vector::Vec2 * pCenter, Vector::Vec2 pRect, float radius, float SizeX, float SizeY)
		//{
		//	return false;
		//}
		//
		//bool Math::Math::NewTestRectangleToRectangle(Vector::Vec2 * pRect0, Vector::Vec2 pRect1, float SizeX0, float SizeX1, float SizeY0, float SizeY1)
		//{
		//	return false;
		//}

		float Math::Math::NeWStaticPointToStaticLineSegment(Vector::Vec2 * pPos, LineSegment * pLine)
		{
			//return distance between line segment and point
			const float length = (pLine1->x - pLine0->x)*(pLine1->x - pLine0->x) + (pLine1->y - pLine0->y) * (pLine1->y - pLine0->y);
			if (length == 0.0f)
			{
				//pline0 = pline1
				return sqrt((pPos->x - pLine0->x)*(pPos->x - pLine0->x) + (pPos->y - pLine0->y) * (pPos->y - pLine0->y));
			}
			const float t = max(0, min(1, Vector::Vector2DDotProduct(pPos - pLine0, pLine1 - pLine0) / length));
			const Vec2 projection = v + t * (pLine1 - pLine0);
			return Vector::Vector2DDistance(pPos, projection);
		}

		//float Math::Math::NewMovingPointToStaticLineSegment(Vector::Vec2 * pStart, Vector::Vec2 * pEnd, LineSegment * pLine, Vector::Vec2 * pInter)
		//{
		//	return 0.0f;
		//}

		//float Math::Math::NewMovingCircleToStaticLineSegment(Vector::Vec2 * pStart, Vector::Vec2 * pEnd, float radius, LineSegment * pLine, Vector::Vec2 * pInter)
		//{
		//	return 0.0f;
		//}

		//float Math::Math::NewReflectMovingPointToStaticLineSegment(Vector::Vec2 * pStart, Vector::Vec2 * pEnd, LineSegment * pLine, Vector::Vec2 * pInter, Vector::Vec2 * pReflect)
		//{
		//	return 0.0f;
		//}
		//
		//float Math::Math::NewMovingPointToStaticCircle(Vector::Vec2 * pStart, Vector::Vec2 * pEnd, Vector::Vec2 * pCenter, float radius, Vector::Vec2 * pInter)
		//{
		//	return 0.0f;
		//}
		//
		//float Math::Math::NewReflectMovingPointToStaticCircle(Vector::Vec2 * pStart, Vector::Vec2 * pEnd, Vector::Vec2 * pCenter, float radius, Vector::Vec2 * pInter, Vector::Vec2 * pReflect)
		//{
		//	return 0.0f;
		//}

		//float Math::Math::NewMovingCircleToStaticCircle(Vector::Vec2 * pCenter0start, Vector::Vec2 * pCenter0end, float radius0, Vector::Vec2 * pCenter1, float radius1, Vector::Vec2 * pInter)
		//{
		//	return 0.0f;
		//}
		//
		//float Math::Math::NewReflectMovingCircleOnStaticCircle(Vector::Vec2 * pCenter0, float radius0, Vector::Vec2 * pCenter1, float radius1)
		//{
		//	return 0.0f;
		//}

	};
}