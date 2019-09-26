#include "Main.h"

/*****************************************************************************

						ORIENTED BOUNDING BOX

*****************************************************************************/
OBB::OBB() : m_center{ 0 }, m_angle{ 0.0f }, m_scale{ 0.0f }, m_minOnAxis{ 0.0f }, m_maxOnAxis{ 0.0f }, // Conversion constructor
m_sides{ SHAPE_NONE }
{}


OBB::~OBB()
{}


void OBB::initCollider(const std::vector<Vec2>& corners)
{
	m_collider = corners;
	m_sides = static_cast<ShapeType>(corners.size());

	m_vertices.resize(m_collider.size());
	m_normals.resize(m_collider.size());
}


void OBB::updateCollider(const Vec2 pos, const float rot, const float size)
{
	m_center = pos;
	m_angle = rot;
	m_scale = size;
	updateVertices();
	updateNormals();
}


void OBB::updateVertices()
{

	Mtx33 rot, sca;
	Mtx33RotRad(rot, m_angle);
	Mtx33Scale(sca, m_scale, m_scale);

	for (int i = 0; i < m_sides; ++i)
	{
		m_vertices[i] = rot * m_collider[i];
		m_vertices[i] = sca * m_vertices[i];
		m_vertices[i] = m_vertices[i] + m_center;
	}
}


inline void OBB::updateNormals()
{
	int i = 0;

	for (; i < m_sides - 1; ++i) // Traverse to the second last vertex
		m_normals[i] = Vec2NormalOf(m_vertices[i + 1] - m_vertices[i]); // n1 till second last normal

	m_normals[i] = Vec2NormalOf(m_vertices[0] - m_vertices[i]);
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


bool OBB::CollisionTest(OBB rhs)
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


bool OBB::checkOverlaps(const OBB &rhs) const
{
	return isBetweenBounds(rhs.m_minOnAxis, m_minOnAxis, m_maxOnAxis) || isBetweenBounds(m_minOnAxis, rhs.m_minOnAxis, rhs.m_maxOnAxis);
}

inline bool OBB::isBetweenBounds(float val, float lowerBound, float upperBound) const
{
	return lowerBound <= val && val <= upperBound;
}