/*Start Header*************************************************************************/
/*!
\file           Polygon.cpp
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
#include "Precompiled.h"
#include "Polygon.h"

namespace Rogue
{
	Polygon::Polygon(PolygonList numSides)
		: m_numSides{ numSides }
	{}

	Polygon::~Polygon()
	{}

	inline PolygonList Polygon::getNumSides() const
	{
		return m_numSides;
	}

	inline void Polygon::setNumSides(PolygonList sides)
	{
		m_numSides = sides;
	}

	//inline std::vector<Vec2> Polygon::getRelVertices() const
	//{
	//	return m_relVertices;
	//}
	//
	//inline void Polygon::setRelVertices(std::vector<Vec2>& vertices)
	//{
	//	m_relVertices = vertices;
	//}
}