/* Start Header ************************************************************************/
/*!
\file           Matrix33.cpp
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
#include "Precompiled.h"
#include "REMath.h"
#include "Vector2D.h"
#include "Matrix33.h"


namespace Rogue
{
	const int TOTAL_VALUES = 9;
	const int N = 3; // Last coordinate of a row/column

	/*************************************************************************/
	/*!
		Conversion constructor for the 3x3 matrix. Initializes the matrix to
		the values in the array.
	*/
	/**************************************************************************/
	Matrix3x3::Matrix3x3(const float* pArr)
	{
		for (int i = 0; i < TOTAL_VALUES; ++i)
			m[i] = pArr[i];
	}

	/**************************************************************************/
	/*!
		Conversion constructor of 3x3 matrix. Initializes the matrix to the
		respective values.
	*/
	/**************************************************************************/
	Matrix3x3::Matrix3x3(float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22)
	{
		m00 = _00;
		m01 = _01;
		m02 = _02;
		m10 = _10;
		m11 = _11;
		m12 = _12;
		m20 = _20;
		m21 = _21;
		m22 = _22;
	}

	/**************************************************************************/
	/*!
		This operator copies the coordinates in the matrix rhs into the current
		matrix.
	*/
	/**************************************************************************/
	Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs)
	{
		for (int i = 0; i < TOTAL_VALUES; ++i)
			m[i] = rhs.m[i];

		return *this;
	}

	/**************************************************************************/
	/*!
		This operator multiplies the current matrix with the matrx rhs
		and returns the result as a matrix.
	*/
	/**************************************************************************/
	Matrix3x3& Matrix3x3::operator *= (const Matrix3x3& rhs)
	{
		Matrix3x3 temp = *this;

		// Add the products of the first row with the first column, and so on.
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
			{
				float product = 0;
				for (int k = 0; k < N; ++k)
					product += temp.m[N * i + k] * rhs.m[N * k + j];

				m[N * i + j] = product;

				product = 0;
			}

		return *this;
	}
	std::ostream& operator<<(std::ostream& out, const Mtx33& matrix)
	{
		return out << matrix.m00 << ", "
			<< matrix.m01 << ", "
			<< matrix.m02 << ", " << "\n"
			<< matrix.m10 << ", "
			<< matrix.m11 << ", "
			<< matrix.m12 << ", " << "\n"
			<< matrix.m20 << ", "
			<< matrix.m21 << ", "
			<< matrix.m22 << ", ";
	}
	/**************************************************************************/
	/*!
		This operator multiplies the matrix lhs with the matrix rhs
		and returns the result as a matrix.
	*/
	/**************************************************************************/
	Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		Matrix3x3 result;
		// Add the products of the first row with the first column, and so on.
		result = lhs;
		result *= rhs;

		return result;
	}

	/**************************************************************************/
	/*!
		This operator multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
	*/
	/**************************************************************************/
	Vector2D  operator * (const Matrix3x3& pMtx, const Vector2D& rhs)
	{
		Vector2D result;

		// Add the products of the first row with the first column, and so on.
		result.x = pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02;
		result.y = pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12;

		return result;
	}

	/**************************************************************************/
	/*!
		This function sets the matrix pResult to the identity matrix
	*/
	/**************************************************************************/
	void Mtx33Identity(Matrix3x3& pResult)
	{
		pResult = { 1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 1.0f };
	}

	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx33Translate(Matrix3x3& pResult, float x, float y)
	{
		// Create identity matrix, then place the x and y values.
		Mtx33Identity(pResult);
		pResult.m02 = x;
		pResult.m12 = y;
	}

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx33Scale(Matrix3x3& pResult, float x, float y)
	{
		// Create identity matrix, then place the x and y values.
		Mtx33Identity(pResult);
		pResult.m00 = x;
		pResult.m11 = y;
	}

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx33RotRad(Matrix3x3& pResult, float angle)
	{
		// Create identity matrix, then place the 4 trigonometrical values

		float cosine = static_cast<float>(cos(angle));
		float sine = static_cast<float>(sin(angle));

		Mtx33Identity(pResult);

		pResult.m00 = cosine;
		pResult.m01 = -sine;
		pResult.m10 = sine;
		pResult.m11 = cosine;

	}

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx33RotDeg(Matrix3x3& pResult, float angle)
	{
		// Convert angle to radian, then do rotation transformation in radians.
		angle *= static_cast<float>(RE_PI / 180);
		Mtx33RotRad(pResult, angle);
	}

	/**************************************************************************/
	/*!
		This function calculates the transpose matrix of pMtx
		and saves it in pResult.
	*/
	/**************************************************************************/
	void Mtx33Transpose(Matrix3x3& pResult, const Matrix3x3& pMtx)
	{
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
			{
				// Flip the values from x and y to y and x
				pResult.m[3 * i + j] = pMtx.m[3 * j + i];
			}
	}


	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void Mtx33Inverse(Matrix3x3& pResult, float& determinant, const Matrix3x3& pMtx)
	{
		Matrix3x3 temp;
		float det00, det01, det02; // Determinants of the middle and bottom 2x2 sections of the matrix.
		float det10, det11, det12; // Determinants of the top and bottom 2x2 sections of the matrix.
		float det20, det21, det22; // Determinants of the top and middle 2x2 sections of the matrix.

		///////////////////// Matrix of Minors /////////////////////
		// Determinants for bottom 2
		det00 = pMtx.m11 * pMtx.m22 - pMtx.m21 * pMtx.m12;
		det01 = pMtx.m10 * pMtx.m22 - pMtx.m20 * pMtx.m12;
		det02 = pMtx.m10 * pMtx.m21 - pMtx.m20 * pMtx.m11;

		// Det = a(det12) - b(det02) + c(det01)
		determinant = pMtx.m00 * det00 - pMtx.m01 * det01 + pMtx.m02 * det02;

		if (determinant <= RE_EPSILON && determinant >= -RE_EPSILON)
		{
			pResult = NULL;
			return;
		}

		// Determinants for top and bottom rows
		det10 = pMtx.m01 * pMtx.m22 - pMtx.m21 * pMtx.m02;
		det11 = pMtx.m00 * pMtx.m22 - pMtx.m20 * pMtx.m02;
		det12 = pMtx.m00 * pMtx.m21 - pMtx.m20 * pMtx.m01;


		// Determinants for top 2 rows
		det20 = pMtx.m01 * pMtx.m12 - pMtx.m11 * pMtx.m02;
		det21 = pMtx.m00 * pMtx.m12 - pMtx.m10 * pMtx.m02;
		det22 = pMtx.m00 * pMtx.m11 - pMtx.m10 * pMtx.m01;


		///////////////////// Matrix of Cofactors /////////////////////
		temp = { det00, -det01,  det02,
					-det10,  det11, -det12,
					det20, -det21,  det22 };

		/////////////////////  Adjointed Matrix   /////////////////////
		Mtx33Transpose(pResult, temp);

		/////////////////////   Inverse Matrix    /////////////////////
		for (int i = 0; i < TOTAL_VALUES; ++i)
			pResult.m[i] *= 1 / determinant;
	}

	Mtx33 Mtx33CreateSRTMatrix(const Vec2& scale, float rotation, const Vec2& translate)
	{
		Mtx33 sca;
		Mtx33 rot;
		Mtx33 trans;

		Mtx33Scale(sca, scale.x, scale.y);
		Mtx33RotRad(rot, rotation);
		Mtx33Translate(trans, translate.x, translate.y);

		return Mtx33(trans * rot * sca);
	}
}