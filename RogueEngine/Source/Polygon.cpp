#pragma once
#include <vector>
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
