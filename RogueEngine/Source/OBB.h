/* Start Header ************************************************************************/
/*!
\file           OBB.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for OBB

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Vector2D.h"
#include "BaseCollider.h"
#include <vector>

namespace Rogue
{
	class OBB : public BaseCollider
	{
	public:
		using VertexList = std::vector<Vec2>;
	private:
		size_t m_size;

		// The minimum and maximum values crushed onto each axis
		float m_minOnAxis;
		float m_maxOnAxis;

		VertexList m_modelVertices;
		VertexList m_globalVertices; // The positions of each vertex in the shape.
		VertexList m_normals; // The normal vectors of each side.
	public:
		OBB(const VertexList& model = VertexList());
		OBB(const OBB& rhs) = default;
		OBB(OBB&& rhs) noexcept; 
		~OBB() = default; // Destructor
		
		OBB& operator=(const OBB&) = default; // Copy assignment
		OBB& operator=(OBB&&) noexcept; // Move assignment

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
}