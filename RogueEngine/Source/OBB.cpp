/* Start Header ************************************************************************/
/*!
\file           OBB.cpp
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
#include "Precompiled.h"
#include "OBB.h"
#include "Polygon.h"
#include "Matrix33.h"

namespace Rogue
{
	OBB::OBB(const OBB::VertexList& model)
		: m_minOnAxis{ 0.0f },
		m_maxOnAxis{ 0.0f },
		m_modelVertices{ model },
		m_globalVertices{ OBB::VertexList(model.size()) },
		m_normals{ OBB::VertexList(model.size()) },
		m_size{ model.size() }
	{
		if (m_size == 0)
		{
			m_normals.reserve(m_size), m_globalVertices.reserve(m_size);

			m_modelVertices.push_back(Vec2(0.5f, 0.5f));
			m_modelVertices.push_back(Vec2(-0.5f, 0.5f));
			m_modelVertices.push_back(Vec2(-0.5f, 0.5f));
			m_modelVertices.push_back(Vec2(0.5f, -0.5f));
		}
	}

	OBB::OBB(OBB&& rhs) noexcept :
		m_minOnAxis{ 0.0f }, m_maxOnAxis{ 0.0f }, m_size{ 0 },
		m_modelVertices{ VertexList{} }, m_globalVertices{ VertexList{} }, m_normals{ VertexList{} }
	{
		std::swap(m_minOnAxis, rhs.m_minOnAxis);
		std::swap(m_maxOnAxis, rhs.m_maxOnAxis);
		std::swap(m_modelVertices, rhs.m_modelVertices);
		std::swap(m_globalVertices, rhs.m_globalVertices);
		std::swap(m_normals, rhs.m_normals);
		std::swap(m_size, rhs.m_size);
	}

	OBB& OBB::operator=(OBB&& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::swap(m_minOnAxis, rhs.m_minOnAxis);
			std::swap(m_maxOnAxis, rhs.m_maxOnAxis);
			std::swap(m_modelVertices, rhs.m_modelVertices);
			std::swap(m_globalVertices, rhs.m_globalVertices);
			std::swap(m_normals, rhs.m_normals);
			std::swap(m_size, rhs.m_size);
		}
		return *this;
	}

	size_t OBB::getSize() const
	{
		return m_size;
	}

	float OBB::getMin() const
	{
		return m_minOnAxis;
	}

	float OBB::getMax() const
	{
		return m_maxOnAxis;
	}

	OBB::VertexList& OBB::globVerts()
	{
		return m_globalVertices;
	}

	OBB::VertexList& OBB::modelVerts()
	{
		return m_modelVertices;
	}

	OBB::VertexList& OBB::normals()
	{
		return m_normals;
	}

	void OBB::setModelVerts(VertexList& model)
	{
		m_modelVertices = model;
		m_globalVertices = OBB::VertexList(model.size());
		m_normals = OBB::VertexList(model.size());
	}

	void OBB::setSize(size_t size)
	{
		m_size = size;
	}

	void OBB::setMin(float min)
	{
		m_minOnAxis = min;
	}

	void OBB::setMax(float max)
	{
		m_maxOnAxis = max;
	}
}