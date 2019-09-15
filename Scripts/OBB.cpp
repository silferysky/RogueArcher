#include "Obb.h"

/*****************************************************************************

						ORIENTED BOUNDING BOX

*****************************************************************************/
OBB::OBB() : m_minOnAxis{ 0.0f }, m_maxOnAxis{ 0.0f } // Conversion constructor
{}


OBB::~OBB()
{}


inline void OBB::update(const Vec2 pos, const std::vector<Vec2>& relVerts, const PolygonList sides, const float angle, const float size)
{
	updateVertices(pos, relVerts, sides, angle, size);
	updateNormals(sides);
}


inline void OBB::updateVertices(const Vec2 pos, const std::vector<Vec2>& relVerts, const PolygonList sides, const float angle, const float size)
{
	Mtx33 rot, sca;
	Mtx33RotRad(rot, angle);
	Mtx33Scale(sca, size, size);

	for(int i = 0; i < sides; i++)
	{
		m_vertices[i] = rot * relVerts[i];
		m_vertices[i] = sca * relVerts[i];
		m_vertices[i] = m_vertices[i] + pos;
	}
}


inline void OBB::updateNormals(const PolygonList sides)
{
	int i;

	for (i = 0; i < sides - 1; ++i) // Traverse to the second last vertex
		m_normals[i] = Vec2NormalOf(m_vertices[i + 1] - m_vertices[i]); // n1 till second last normal

	m_normals[i] = Vec2NormalOf(m_vertices[0] - m_vertices[i]); // Last normal
}


void OBB::SATFindMinMax(const Vec2 &normal)
{
	m_minOnAxis = FLT_MAX; // Initialize as the max float value
	m_maxOnAxis = -m_minOnAxis;	   // Initialize as the min float value

	for (Vec2 vertex : m_vertices) // Dot every vertex with the current normal
	{
		float result = Vec2DotProd(vertex, normal);

		// We only need to find the extreme mins and maxes of the shape.
		if (result < m_minOnAxis)
			m_minOnAxis = result;

		if (result > m_maxOnAxis)
			m_maxOnAxis = result;
	}
}


inline bool OBB::CollisionTest(OBB rhs)
{
	for (Vec2 normal : m_normals)
	{
		SATFindMinMax(normal);
		rhs.SATFindMinMax(normal);

		if (checkOverlaps(rhs) == false)
			return false; // No intersection
	}

	for (Vec2 normal : rhs.m_normals)
	{
		SATFindMinMax(normal);
		rhs.SATFindMinMax(normal);

		if (rhs.checkOverlaps(*this) == false)
			return false; // No intersection
	}

	return true;
}


inline bool OBB::checkOverlaps(const OBB &rhs) const
{
	return isBetweenBounds(rhs.m_minOnAxis, m_minOnAxis, m_maxOnAxis) || isBetweenBounds(m_minOnAxis, rhs.m_minOnAxis, rhs.m_maxOnAxis);
}

inline bool OBB::isBetweenBounds(float val, float lowerBound, float upperBound) const
{
	return lowerBound <= val && val <= upperBound;
}