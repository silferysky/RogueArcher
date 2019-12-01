/*Start Header*************************************************************************/
/*!
\file           Polygon.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Polygon

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Vector2D.h"

namespace Rogue
{
	enum PolygonList
	{
		POLY_NONE = 0,
		POLY_POINT,
		POLY_LINE,
		POLY_TRI,
		POLY_QUAD,
		POLY_PENT,
		POLY_HEXA,
		POLY_HEPT,
		POLY_OCTA,
		POLY_NONA,
		POLY_DECA
	};

	// Polygon class
	class Polygon
	{
		PolygonList m_numSides;
		std::vector<Vec2> m_relVertices;

	public:
		Polygon(PolygonList numSides = POLY_NONE);
		~Polygon();

		inline PolygonList getNumSides() const;
		inline void setNumSides(PolygonList sides);
		inline std::vector<Vec2> getRelVertices() const;
		inline void setRelVertices(std::vector<Vec2>& vertices);

	};
}