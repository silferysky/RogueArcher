#include <cmath>
#include <algorithm>
#include "REMath.h"

namespace Rogue
{
	void REMath::REBuildLineSegment(LineSegment& lineSegment,								//Line segment reference - output 
		const Vec2& pos,							//position - input 
		float scale,											//scale - input 
		float dir)											//direction angle - input
	{
		// Points pt0 and pt1
		lineSegment.m_pt0 = { pos.x - 0.5f * scale, pos.y };
		lineSegment.m_pt1 = { pos.x + 0.5f * scale, pos.y };

		Mtx33 rot;
		Mtx33RotRad(rot, dir);

		// Rotate the line to obtain P0 and P1
		lineSegment.m_pt0 = pos + rot * (lineSegment.m_pt0 - pos);
		lineSegment.m_pt1 = pos + rot * (lineSegment.m_pt1 - pos);

		// Vector of the line
		Vec2 temp = lineSegment.m_pt1 - pos;

		// Normalize the vector
		lineSegment.m_normal = { temp.y, -temp.x };
		Vec2Normalize(lineSegment.m_normal, lineSegment.m_normal);

	}

	double REMath::DegreesToRadians(double degrees)
	{
		return (degrees * REMath::PI / 180.0);
	}

	double REMath::RadiansToDegrees(double radians)
	{
		return (radians * 180.0 / REMath::PI);
	}

	float REMath::RESin(float x)
	{
		float result = sin(x);
		return result;
	}

	float REMath::RECos(float x)
	{
		float result = cos(x);
		return result;
	}

	float REMath::RETan(float x)
	{
		float result = tan(x);
		return result;
	}

	float REMath::REASin(float x)
	{
		float result = asin(x);
		return result;
	}

	float REMath::REACos(float x)
	{
		float result = acos(x);
		return result;
	}

	float REMath::REATan(float x)
	{
		float result = atan(x);
		return result;
	}

	double REMath::RELogBase2(double x)
	{
		double result = log2(x);
		return result;
	}

	float REMath::REClamp(float x, float x0, float x1)
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

	float REMath::REWrap(float x, float x0, float x1)
	{
		float range = x1 - x0;
		if (x < (x0 - range) || (x > (x1 + range)))
		{
			return x;
		}

		else
		{
			if (x < x0)
			{
				return (x + range);
			}
			else
			{
				return (x - range);
			}
		}
	}

	float REMath::REMin(float x, float y)
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

	float REMath::REMax(float x, float y)
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

	bool REMath::REInRange(float x, float x0, float x1)
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

	float REMath::REDistPointToCircle(Vec2& pPos, Vec2& pCenter, float radius)
	{
		float result;
		float x1 = (pPos.x - pCenter.x) * (pPos.x - pCenter.x);
		float y1 = (pPos.y - pCenter.y) * (pPos.y - pCenter.y);
		result = sqrt(x1 + y1) - radius;
		return result;
	}

	float REMath::REDistPointToRectangle(Vec2& pPos, Vec2& pRect, float wigDeltaTimeh, float height)
	{
		float dx, dy;
		dx = REMax(abs(pPos.x - pRect.x) - wigDeltaTimeh / 2, 0.0f);
		dy = REMax(abs(pPos.x - pRect.x) - height / 2, 0.0f);
		return (dx * dx + dy * dy);
	}

	float REMath::REDistPointToLineSegment(const Vec2& pPos, const Vec2& pLine0, const Vec2& pLine1)
	{
		float distance1 = pLine1.x - pLine0.x;
		float distance2 = pLine1.y - pLine0.y;
		float distance3 = pLine1.x * pLine0.y - pLine1.y * pLine0.x;

		return abs(distance1 * pPos.x + distance2 * pPos.y + distance3) / (distance1 * distance1 + distance2 * distance2);
	}

	float REMath::REDistCircleToCircle(Vec2& pCenter0, Vec2& pCenter1, float radius0, float radius1)
	{
		float distance = Vec2Distance(pCenter0, pCenter1) - (radius0 + radius1);
		return distance;
	}

	float REMath::REDistCircleToRectangle(Vec2& pCenter, Vec2& pRect, float radius, float SizeX, float SizeY)
	{
		float DeltaX, DeltaY;
		DeltaX = pCenter.x - REMax(pRect.x, REMin(pCenter.x, pRect.x + SizeX));
		DeltaY = pCenter.y - REMax(pRect.y, REMin(pCenter.y, pRect.y + SizeY));
		return (DeltaX * DeltaX + DeltaY * DeltaY) < (radius * radius);
	}

	float REMath::REDistRectangleToRectangle(const Vec2& pRect0, float SizeX0, float SizeY0, const Vec2& pRect1, float SizeX1, float SizeY1)
	{
		return REMax(abs(pRect1.x - pRect0.x) - (SizeX0 + SizeX1) / 2, abs(pRect1.y - pRect0.y) - (SizeY1 + SizeY0) / 2);
	}



	//bool RETestPointToCircle(Vec2& pPos, Vec2& pCenter, float radius)
	//{
	//	return false;
	//}
	//
	//bool RETestPointToRectangle(Vec2& pPos, Vec2& pRect, float SizeX, float SizeY)
	//{
	//	return false;
	//}
	//
	//bool RETestCircleToCircle(Vec2& pCenter0, Vec2& pCenter1, float radius0, float radius1)
	//{
	//	return false;
	//}
	//
	//bool RETestCircleToRectangle(Vec2& pCenter, Vec2 pRect, float radius, float SizeX, float SizeY)
	//{
	//	return false;
	//}
	//
	//bool RETestRectangleToRectangle(Vec2& pRect0, Vec2 pRect1, float SizeX0, float SizeX1, float SizeY0, float SizeY1)
	//{
	//	return false;
	//}

	//float REMath::REStaticPointToStaticLineSegment(Vec2& pPos, REMath::LineSegment& pLine)
	//{
	//	//return distance between line segment and point
	//	const float length = (pLine1.x - pLine0.x) * (pLine1.x - pLine0.x) + (pLine1.y - pLine0.y) * (pLine1.y - pLine0.y);
	//	if (length == 0.0f)
	//	{
	//		//pline0 = pline1
	//		return sqrt((pPos.x - pLine0.x)*(pPos.x - pLine0.x) + (pPos.y - pLine0.y) * (pPos.y - pLine0.y));
	//	}
	//	const float t = REMax(0, REMin(1, Vec2DotProd(pPos - pLine0, pLine1 - pLine0) / length));
	//	const Vec2 projection = v + t * (pLine1 - pLine0);
	//	return Vec2Distance(pPos, projection);
	//}

	//float REMovingPointToStaticLineSegment(Vec2& pStart, Vec2& pEnd, LineSegment * pLine, Vec2& pInter)
	//{
	//	return 0.0f;
	//}

	//float REMovingCircleToStaticLineSegment(Vec2& pStart, Vec2& pEnd, float radius, LineSegment * pLine, Vec2& pInter)
	//{
	//	return 0.0f;
	//}

	//float REReflectMovingPointToStaticLineSegment(Vec2& pStart, Vec2& pEnd, LineSegment * pLine, Vec2& pInter, Vec2& pReflect)
	//{
	//	return 0.0f;
	//}
	//
	//float REMovingPointToStaticCircle(Vec2& pStart, Vec2& pEnd, Vec2& pCenter, float radius, Vec2& pInter)
	//{
	//	return 0.0f;
	//}
	//
	//float REReflectMovingPointToStaticCircle(Vec2& pStart, Vec2& pEnd, Vec2& pCenter, float radius, Vec2& pInter, Vec2& pReflect)
	//{
	//	return 0.0f;
	//}

	//float REMovingCircleToStaticCircle(Vec2& pCenter0start, Vec2& pCenter0end, float radius0, Vec2& pCenter1, float radius1, Vec2& pInter)
	//{
	//	return 0.0f;
	//}
	//
	//float REReflectMovingCircleOnStaticCircle(Vec2& pCenter0, float radius0, Vec2& pCenter1, float radius1)
	//{
	//	return 0.0f;
	//}
}

