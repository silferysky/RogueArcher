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
	{}

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
