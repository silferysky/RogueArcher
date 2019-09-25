#include <cmath>
#include "Vector2D.h"
#include "Matrix3x3.h"

namespace Math
{
	class Math
	{
		private:
		double DegreesToRadians(double degrees) const;
		double RadiansToDegrees(double radians) const;
		struct LineSegment
		{
			Vector::Vec2	m_pt0;
			Vector::Vec2	m_pt1;
			Vector::Vec2	m_normal;
		};
		public:
		//building line segment
		void BuildLineSegment(LineSegment &lineSegment,const Vector::Vec2 &pos,float scale,float dir);
		//calcuations
		float NewSin(float x);
		float NewCos(float x);
		float NewTan(float x);
		float NewASin(float x);
		float NewACos(float x);
		float NewATan(float x);
		double NewLogBase2(double x);

		//clamp x to be between x0 and x1
		float NewClamp(float x, float x0, float x1);
		//Wrap x with respect to the range of x0 and x1, does not work if x is lesser than (x0 - range) or if x is greater than(x1 + range).
		float NewWrap(float x, float x0, float x1);
		//find which values are higher or lower and return it
		float NewMin(float x, float y);
		float NewMax(float x, float y);
		//check whether values are in range
		bool NewInRange(float x, float x0, float x1);
		//check for distance
		float NewDistPointToCircle(Vector::Vec2* pPos, Vector::Vec2* pCenter, float radius);
		float NewDistPointToRectangle(Vector::Vec2* pPos, Vector::Vec2* pRect, float x, float y);
		float NewDistPointToLineSegment(Vector::Vec2* pPos, Vector::Vec2 pLine0, Vector::Vec2 pLine1);

		float NewDistCircleToCircle(Vector::Vec2* pCenter0, Vector::Vec2* pCenter1, float radius0, float radius1);
		float NewDistCircleToRectangle(Vector::Vec2* pCenter, Vector::Vec2* pRect, float radius, float SizeX, float SizeY);

		float NewDistRectangleToRectangle(Vector::Vec2* pRect0, float SizeX0, float SizeY0, Vector::Vec2* pRect1, float SizeX1, float SizeY1, Vector::Vec2* pNormal);
		//collision detection
		bool NewTestPointToCircle(Vector::Vec2* pPos, Vector::Vec2* pCenter, float radius);
		bool NewTestPointToRectangle(Vector::Vec2* pPos, Vector::Vec2* pRect, float SizeX, float SizeY);
		bool NewTestCircleToCircle(Vector::Vec2* pCenter0, Vector::Vec2* pCenter1, float radius0, float radius1);
		bool NewTestCircleToRectangle(Vector::Vec2* pCenter, Vector::Vec2 pRect, float radius, float SizeX, float SizeY);
		bool NewTestRectangleToRectangle(Vector::Vec2* pRect0, Vector::Vec2 pRect1, float SizeX0, float SizeX1, float SizeY0, float SizeY1);
		//distance for line segments
		float NeWStaticPointToStaticLineSegment(Vector::Vec2* pPos, LineSegment *pLine);
		float NewMovingPointToStaticLineSegment(Vector::Vec2* pStart, Vector::Vec2 *pEnd, LineSegment *pLine, Vector::Vec2 *pInter);
		float NewMovingCircleToStaticLineSegment(Vector::Vec2* pStart, Vector::Vec2 *pEnd, float radius, LineSegment *pLine, Vector::Vec2 *pInter);
		float NewReflectMovingPointToStaticLineSegment(Vector::Vec2* pStart, Vector::Vec2* pEnd, LineSegment *pLine, Vector::Vec2 *pInter, Vector::Vec2 *pReflect);
		float NewMovingPointToStaticCircle(Vector::Vec2* pStart, Vector::Vec2* pEnd, Vector::Vec2* pCenter, float radius, Vector::Vec2* pInter);
		float NewReflectMovingPointToStaticCircle(Vector::Vec2* pStart, Vector::Vec2* pEnd, Vector::Vec2* pCenter, float radius, Vector::Vec2 *pInter, Vector::Vec2* pReflect);
		float NewMovingCircleToStaticCircle(Vector::Vec2* pCenter0start, Vector::Vec2* pCenter0end, float radius0, Vector::Vec2 *pCenter1, float radius1, Vector::Vec2* pInter);
		float NewReflectMovingCircleOnStaticCircle(Vector::Vec2 *pCenter0, float radius0, Vector::Vec2* pCenter1, float radius1);



	};
}