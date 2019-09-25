#pragma once
#include <vector>
#include "../Scripts/Matrix33.h"
#include "Polygon.h"

class OBB
{
	using VerticesList = std::vector<Vec2>;

	// The minimum and maximum values crushed onto each axis
	float m_minOnAxis;
	float m_maxOnAxis;

	VerticesList m_modelVertices;
	VerticesList m_globalVertices; // The positions of each vertex in the shape.
	VerticesList m_normals; // The normal vectors of each side.
public:
	OBB() = default;
	~OBB() = default;

	float getMin() const;
	float getMax() const;
	VerticesList& globVerts(); // Get global Vertices
	VerticesList& modelVerts();
	VerticesList& normals();

	void setMin(float min);
	void setMax(float max);
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