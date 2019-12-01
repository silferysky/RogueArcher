/* Start Header ************************************************************************/
/*!
\file           Matrix33.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Matrix33

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


namespace Rogue
{
	typedef union Matrix3x3
	{
		struct
		{
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
		};

		float m[9] = { 0.0f };

		Matrix3x3() :
			m{ 1.0f, 0.0f, 0.0f,
			   0.0f, 1.0f, 0.0f,
			   0.0f, 0.0f, 1.0f }
		{}
		Matrix3x3(const float* pArr);
		Matrix3x3(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
		Matrix3x3& operator=(const Matrix3x3& rhs);

		// Assignment operators
		Matrix3x3& operator *= (const Matrix3x3& rhs);

	} Matrix3x3, Mtx33;


	std::ostream& operator << (std::ostream& out, const Mtx33& matrix);

	Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs);

	/**************************************************************************/
	/*!
		This operator multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
		*/
		/**************************************************************************/
	Vector2D  operator * (const Matrix3x3& pMtx, const Vector2D& rhs);

	/**************************************************************************/
	/*!
		This function sets the matrix pResult to the identity matrix
		*/
		/**************************************************************************/
	void Mtx33Identity(Matrix3x3& pResult);

	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
		*/
		/**************************************************************************/
	void Mtx33Translate(Matrix3x3& pResult, float x, float y);

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
		*/
		/**************************************************************************/
	void Mtx33Scale(Matrix3x3& pResult, float x, float y);

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
		*/
		/**************************************************************************/
	void Mtx33RotRad(Matrix3x3& pResult, float angle);

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
		*/
		/**************************************************************************/
	void Mtx33RotDeg(Matrix3x3& pResult, float angle);

	/**************************************************************************/
	/*!
		This functions calculated the transpose matrix of pMtx
		and saves it in pResult
		*/
		/**************************************************************************/
	void Mtx33Transpose(Matrix3x3& pResult, const Matrix3x3& pMtx);

	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void Mtx33Inverse(Matrix3x3& pResult, float& determinant, const Matrix3x3& pMtx);
	
	Mtx33 Mtx33CreateSRTMatrix(const Vec2& scale, float rotation, const Vec2& translate);
}