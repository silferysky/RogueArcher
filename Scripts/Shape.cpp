#include "Physics.h"

Shape::~Shape()
{
}

void Shape::initShape(float w, float h, float m)
{
	m_width = w;
	m_height = h;
	m_mass = m;

	m_inertia = calcInertia();
}

inline float Shape::calcInertia() const
{
	return m_mass * (m_width * m_width + m_height * m_height) / 12;
}