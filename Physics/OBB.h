#pragma once


class OBB : public BaseCollider
{
	float m_minOnAxis; // To be used to test for collision
	float m_maxOnAxis;

	std::vector<Vec2> m_vertices; // The positions of each vertex in the shape.
	std::vector<Vec2> m_normals; // The normal vectors of each side.

public:
	OBB();
	~OBB();

	inline void update(const Vec2 pos, const std::vector<Vec2>& relVerts, const PolygonList sides, const float angle, const float size);

	inline void updateVertices(const Vec2 pos, const std::vector<Vec2>& relVerts, const PolygonList sides, const float angle, const float size);
	inline void updateNormals(const PolygonList sides);

	void SATFindMinMax(const Vec2 &normal);
	inline bool CollisionTest(OBB rhs);

	inline bool checkOverlaps(const OBB &rhs) const;
	inline bool isBetweenBounds(float val, float lowerBound, float upperBound) const;
};


/*******************************************************************************
How to store the vertices in the OBB:

					   n1
					   ^
					   |
					   |
				p1 ^-------> p2
				   |       |
		n4 <------ |   C   | -------> n2
				   |       |
				p4 <-------v p3
					   |
					   |
					   v
					   n3


********************************************************************************/