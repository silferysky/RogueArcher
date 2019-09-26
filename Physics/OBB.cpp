#include "OBB.h"

float OBB::getMin() const
{
	return m_minOnAxis;
}

float OBB::getMax() const
{
	return m_maxOnAxis;
}

OBB::VerticesList& OBB::globVerts()
{
	return m_globalVertices;
}

OBB::VerticesList& OBB::modelVerts()
{
	return m_modelVertices;
}

OBB::VerticesList& OBB::normals()
{
	return m_normals;
}

void OBB::setMin(float min)
{
	m_minOnAxis = min;
}

void OBB::setMax(float max)
{
	m_maxOnAxis = max;
}