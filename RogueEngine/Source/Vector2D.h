/* Start Header ************************************************************************/
/*!
\file           Vector2D.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Vector2D

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

#include "imgui.h"
#include "vec2.hpp"


namespace Rogue
{
	typedef union Vector2D
	{
		struct
		{
			float x, y;
		};

		float m[2];

		static const Vector2D s_unitX;
		static const Vector2D s_unitY;

		Vector2D() : x{ 0.0f }, y{ 0.0f } // Default constructor
		{}

		Vector2D(int X, int Y); // Conversion constructor
		Vector2D(float X, float Y); // Conversion constructor
		Vector2D(const ImVec2& rhs); // Conversion constructor
		Vector2D(const glm::vec2& rhs); // Conversion constructor
		Vector2D(const Vector2D& rhs); // Copy constructor
		Vector2D(Vector2D&& rhs) noexcept; // Move constructor

		~Vector2D(); // Destructor

		// Assignment operators
		Vector2D& operator =  (const Vector2D& rhs); // Copy assignment
		Vector2D& operator =  (Vector2D&& rhs) noexcept; // Move assignment
		Vector2D& operator += (const Vector2D& rhs);
		Vector2D& operator -= (const Vector2D& rhs);
		Vector2D& operator *= (float rhs);
		Vector2D& operator /= (float rhs);

		// Unary operators
		Vector2D operator -() const; // Inverse

		// Comparison operators
		bool operator == (const Vector2D& rhs) const;

	} Vec2, Point2D, Pt2;

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

// << Operator
	std::ostream& operator << (std::ostream& os, const Vec2& vec);

	// Binary operators
	Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs);
	Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs);
	Vector2D operator * (const Vector2D& lhs, float rhs);
	Vector2D operator * (const float lhs, const Vector2D& rhs);
	Vector2D operator / (const Vector2D& lhs, float rhs);


	// Math functions

	Vector2D	Vec2NormalOf(const Vector2D& pVec);

	float	Vec2Rotation(const Vector2D& ptLast, const Vector2D& ptFirst = Vector2D::s_unitX);

	void	Vec2Normalize(Vector2D& pResult, const Vector2D& pVec0);

	float	Vec2Length(const Vector2D& pVec0);

	float	Vec2SqLength(const Vector2D& pVec0);

	float	Vec2Distance(const Vector2D& pVec0, const Vector2D& pVec1);

	float	Vec2SqDistance(const Vector2D& pVec0, const Vector2D& pVec1);

	float	Vec2DotProduct(const Vector2D& pVec0, const Vector2D& pVec1);

	float	Vec2CrossProd(const Vector2D& pVec0, const Vector2D& pVec1);

}