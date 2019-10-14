#pragma once

#include "Vector2D.h"
#include "Matrix33.h"

namespace REMath
{
	const double EPSILON = 0.000001;
	const double PI = 3.1415926535897;

  struct LineSegment
  {
	  Vec2 m_pt0;
	  Vec2 m_pt1;
	  Vec2 m_normal;
  };

  struct Circle
  {
	  Vec2 m_center;
	  float m_radius;
	  float m_mass;

	  Circle()
		: m_center{ Vec2() },
  		  m_radius{ 1.0f },
		  m_mass{ 1.0f }
	  {}
  };

  struct Ray
  {
	  Vec2 m_pt0;
	  Vec2 m_dir;
  };

  double DegreesToRadians(double degrees);
  double RadiansToDegrees(double radians);

  //building line segment
  void REBuildLineSegment(LineSegment &lineSegment,const Vec2 &pos,float scale,float dir);
  //calcuations
  float RESin(float x);
  float RECos(float x);
  float RETan(float x);
  float REASin(float x);
  float REACos(float x);
  float REATan(float x);
  double RELogBase2(double x);

  //clamp x to be between x0 and x1
  float REClamp(float x, float x0, float x1);
  //Wrap x with respect to the range of x0 and x1, does not work if x is lesser than (x0 - range) or if x is greater than(x1 + range).
  float REWrap(float x, float x0, float x1);
  //find which values are higher or lower and return it
  float REMin(float x, float y);
  float REMax(float x, float y);
  //check whether values are in range
  bool REInRange(float x, float x0, float x1);
  //check for distance
  float REDistPointToCircle(Vec2& pPos, Vec2& pCenter, float radius);
  float REDistPointToRectangle(Vec2& pPos, Vec2& pRect, float x, float y);
  float REDistPointToLineSegment(const Vec2& pPos, const Vec2& pLine0, const Vec2& pLine1);

  float REDistCircleToCircle(Vec2& pCenter0, Vec2& pCenter1, float radius0, float radius1);
  float REDistCircleToRectangle(Vec2& pCenter, Vec2& pRect, float radius, float SizeX, float SizeY);

  float REDistRectangleToRectangle(const Vec2& pRect0, float SizeX0, float SizeY0, const Vec2& pRect1, float SizeX1, float SizeY1);
  //collision detection


  bool RETestPointToCircle(Vec2& pPos, Vec2& pCenter, float radius);
  bool RETestPointToRectangle(Vec2& pPos, Vec2& pRect, float SizeX, float SizeY);
  bool RETestCircleToCircle(Vec2& pCenter0, Vec2& pCenter1, float radius0, float radius1);
  bool RETestCircleToRectangle(Vec2& pCenter, Vec2 pRect, float radius, float SizeX, float SizeY);
  bool RETestRectangleToRectangle(Vec2& pRect0, Vec2 pRect1, float SizeX0, float SizeX1, float SizeY0, float SizeY1);
  //distance for line segments
  float REStaticPointToStaticLineSegment(Vec2& pPos, LineSegment& pLine);
  float REMovingPointToStaticLineSegment(Vec2& pStart, Vec2&pEnd, LineSegment& pLine, Vec2& pInter);
  float REMovingCircleToStaticLineSegment(Vec2& pStart, Vec2&pEnd, float radius, LineSegment& pLine, Vec2&pInter);
  float REReflectMovingPointToStaticLineSegment(Vec2& pStart, Vec2& pEnd, LineSegment& pLine, Vec2&pInter, Vec2&pReflect);
  float REMovingPointToStaticCircle(Vec2& pStart, Vec2& pEnd, Vec2& pCenter, float radius, Vec2& pInter);
  float REReflectMovingPointToStaticCircle(Vec2& pStart, Vec2& pEnd, Vec2& pCenter, float radius, Vec2&pInter, Vec2& pReflect);
  float REMovingCircleToStaticCircle(Vec2& pCenter0start, Vec2& pCenter0end, float radius0, Vec2&pCenter1, float radius1, Vec2& pInter);
  float REReflectMovingCircleOnStaticCircle(Vec2&pCenter0, float radius0, Vec2& pCenter1, float radius1);
};