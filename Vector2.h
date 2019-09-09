/******************************************************************************/
/*!
\file		Vector2D.h
\author 	Lim Jia Hao
\par    	email: l.jiahao\@digipen.edu
\date   	March 27, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

typedef union Vector2D
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	struct
	{
		float x, y;
	};

	float m[2];

	Vector2D() : x{ 0.0f }, y{ 0.0f } // Default constructor
	{}

	Vector2D(const int num); // Conversion constructor
	Vector2D(const float X, const float Y); // Conversion constructor
	Vector2D(const Vector2D & vec); // Copy constructor

	~Vector2D(); // Destructor

	// Assignment operators
	Vector2D& operator =  (const int num);
	Vector2D& operator =  (const Vector2D &rhs);
	Vector2D& operator += (const Vector2D &rhs);
	Vector2D& operator -= (const Vector2D &rhs);
	Vector2D& operator *= (float rhs);
	Vector2D& operator /= (float rhs);

	// Unary operators
	Vector2D operator -() const; // Inverse

} Vec2, Point2D, Pt2;


// Binary operators
Vector2D operator + (const Vector2D &lhs, const Vector2D &rhs);
Vector2D operator - (const Vector2D &lhs, const Vector2D &rhs);
Vector2D operator * (const Vector2D &lhs, float rhs);
Vector2D operator * (float lhs, const Vector2D &rhs);
Vector2D operator / (const Vector2D &lhs, float rhs);


// Math functions

Vector2D	Vec2NormalOf(const Vector2D &pVec);

void	Vec2Normalize(Vector2D &pResult, const Vector2D &pVec0);

float	Vec2Length(const Vector2D &pVec0);

float	Vec2SqLength(const Vector2D &pVec0);

float	Vec2Distance(const Vector2D &pVec0, const Vector2D &pVec1);

float	Vec2SqDistance(const Vector2D &pVec0, const Vector2D &pVec1);

float	Vec2DotProd(const Vector2D &pVec0, const Vector2D &pVec1);

float	Vec2CrossProd(const Vector2D &pVec0, const Vector2D &pVec1);