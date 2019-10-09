#pragma once
#include <vector>
#include "../Source/Matrix33.h"
#include "Polygon.h"

class OBB
{
	using VertexList = std::vector<Vec2>;

	size_t m_size;

	// The minimum and maximum values crushed onto each axis
	float m_minOnAxis;
	float m_maxOnAxis;

	VertexList m_modelVertices;
	VertexList m_globalVertices; // The positions of each vertex in the shape.
	VertexList m_normals; // The normal vectors of each side.
public:
	OBB(const VertexList& model = VertexList());
	~OBB() = default;

	size_t getSize() const;
	float getMin() const;
	float getMax() const;
	VertexList& globVerts(); // Get global Vertices
	VertexList& modelVerts();
	VertexList& normals();

	void setModelVerts(VertexList& model);

	void setSize(size_t size);
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