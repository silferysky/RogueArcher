/* Start Header ************************************************************************/
/*!
\file           Vector2D.cpp
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
#include "Precompiled.h"
#include "Vector2D.h"


namespace Rogue
{
	const Vector2D Vector2D::s_unitX = { 1.0f, 0.0f };
	const Vector2D Vector2D::s_unitY = { 0.0f, 1.0f };

	Vector2D::Vector2D(int X, int Y) :
		x{ static_cast<float>(X) }, y{ static_cast<float>(Y) }
	{}

	Vector2D::~Vector2D()
	{}

	Vector2D::Vector2D(float X, float Y) :
		x{ X }, y{ Y }
	{}

	Vector2D::Vector2D(const ImVec2& rhs) :
		x{ rhs.x }, y{ rhs.y }
	{}

	Vector2D::Vector2D(const glm::vec2& rhs) :
		x{ rhs.x }, y{ rhs.y }
	{}

	Vector2D::Vector2D(const Vector2D& vec) :
		x{ vec.x }, y{ vec.y }
	{}

	Vector2D::Vector2D(Vector2D&& rhs) noexcept :
		x{ 0 }, y{ 0 }
	{
		std::swap(x, rhs.x);
		std::swap(y, rhs.y);
	}

	Vector2D& Vector2D::operator=(const Vector2D& rhs)
	{
		if (&rhs != this)
		{
			x = rhs.x;
			y = rhs.y;
		}

		return *this;
	}

	Vector2D& Vector2D::operator=(Vector2D&& rhs) noexcept
	{
		std::swap(x, rhs.x);
		std::swap(y, rhs.y);

		return *this;
	}


	Vector2D& Vector2D::operator += (const Vector2D& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}


	/**************************************************************************/
	/*!
		Compound subtraction operator of a vector from the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D& Vector2D::operator -= (const Vector2D& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}


	/**************************************************************************/
	/*!
		Compound multiplication operator of a vector from the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D& Vector2D::operator *= (float rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}


	/**************************************************************************/
	/*!
		Compound division operator of a vector from the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D& Vector2D::operator /= (float rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}


	/**************************************************************************/
	/*!
		Unary negation operator of a vector.
	 */
	 /**************************************************************************/
	Vector2D Vector2D::operator -() const
	{
		Vector2D result;
		result.x = -x;
		result.y = -y;

		return result;
	}

	bool Vector2D::operator == (const Vector2D& rhs) const
	{
		if (x == rhs.x && y == rhs.y)
			return true;
		
		return false;
	}

	/**************************************************************************/
	/*!
		Outputs the values of x and y in the vector.
	 */
	 /**************************************************************************/
	std::ostream& operator<<(std::ostream& os, const Vec2& vec)
	{
		return os << "(" << vec.x << "," << vec.y << ")";
	}

	/**************************************************************************/
	/*!
		Binary addition operator of a vector lhs from the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs)
	{
		Vector2D result;

		result.x = lhs.x + rhs.x;
		result.y = lhs.y + rhs.y;

		return result;
	}


	/**************************************************************************/
	/*!
		Binary subtraction operator of a vector lhs from the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs)
	{
		Vector2D result;

		result.x = lhs.x - rhs.x;
		result.y = lhs.y - rhs.y;

		return result;
	}


	/**************************************************************************/
	/*!
		Binary multiplication operator of a vector lhs from the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D operator * (const Vector2D& lhs, float rhs)
	{
		Vector2D result;

		result.x = lhs.x * rhs;
		result.y = lhs.y * rhs;

		return result;
	}


	/**************************************************************************/
	/*!
		Binary multiplication operator of a vector lhs from the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D operator * (float lhs, const Vector2D& rhs)
	{
		Vector2D result;

		result.x = lhs * rhs.x;
		result.y = lhs * rhs.y;

		return result;
	}


	/**************************************************************************/
	/*!
		Binary division operator of a vector lhs by the vector rhs.
	 */
	 /**************************************************************************/
	Vector2D operator / (const Vector2D& lhs, float rhs)
	{
		Vector2D result;

		result.x = lhs.x / rhs;
		result.y = lhs.y / rhs;

		return result;
	}


	Vector2D Vec2NormalOf(const Vector2D& pVec)
	{
		return Vector2D(-pVec.y, pVec.x);
	}

	float Vec2Rotation(const Vector2D& ptLast, const Vector2D& ptFirst)
	{
		float x = ptLast.x - ptFirst.x;
		float y = ptLast.y - ptFirst.y;

		return std::atan2(y, x);
	}

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/
	void	Vec2Normalize(Vector2D& pResult, const Vector2D& pVec0)
	{
		if (pVec0 == Vec2())
		{
			pResult = Vec2();
			return;
		}

		float root = sqrtf(pVec0.x * pVec0.x + pVec0.y * pVec0.y);

		pResult = pVec0 / root;
	}


	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float	Vec2Length(const Vector2D& pVec0)
	{
		return (sqrtf(pVec0.x * pVec0.x + pVec0.y * pVec0.y));
	}


	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	 */
	 /**************************************************************************/
	float	Vec2SqLength(const Vector2D& pVec0)
	{
		return pVec0.x * pVec0.x + pVec0.y * pVec0.y;
	}


	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	 */
	 /**************************************************************************/
	float	Vec2Distance(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		return Vec2Length(pVec1 - pVec0);
	}


	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	 */
	 /**************************************************************************/
	float	Vec2SqDistance(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		return Vec2SqLength(pVec1 - pVec0);
	}


	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vec2DotProduct(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		return pVec0.x * pVec1.x + pVec0.y * pVec1.y;
	}


	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vec2CrossProd(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		// Calulate the value of the z dimension of the cross product.
		return pVec0.x * pVec1.y - pVec0.y * pVec1.x;
	}
}