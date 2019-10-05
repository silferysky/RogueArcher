#pragma once
#include <vector>
#include "../Source/Vector2D.h"

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