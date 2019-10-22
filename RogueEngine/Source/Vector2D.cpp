#include <cmath>
#include "Vector2D.h"


namespace Rogue
{
	Vector2D::Vector2D(const int num) : x{ static_cast<float>(num) }, y{ static_cast<float>(num) }
	{}


	Vector2D::Vector2D(const float X, const float Y) : x{ X }, y{ Y }
	{}


	Vector2D::~Vector2D()
	{}


	Vector2D::Vector2D(const Vector2D& vec)
	{
		x = vec.x;
		y = vec.y;
	}


	Vector2D& Vector2D::operator = (const int num)
	{
		x = (float)num;
		y = (float)num;

		return *this;
	}


	Vector2D& Vector2D::operator=(const Vector2D& rhs)
	{
		x = rhs.x;
		y = rhs.y;

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

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/
	void	Vec2Normalize(Vector2D& pResult, const Vector2D& pVec0)
	{
		pResult = pVec0 / static_cast<float>(sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y));
	}


	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float	Vec2Length(const Vector2D& pVec0)
	{
		return static_cast<float>(sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y));
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
	float	Vec2DotProd(const Vector2D& pVec0, const Vector2D& pVec1)
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