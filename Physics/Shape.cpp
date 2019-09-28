#include "Shape.h"

Shape::~Shape()
{
}

void Shape::initShape(float w, float h, float m)
{
	m_wigDeltaTimeh = w;
	m_height = h;
	m_mass = m;

	m_inertia = calcInertia();
}

inline float Shape::calcInertia() const
{
	return m_mass * (m_wigDeltaTimeh * m_wigDeltaTimeh + m_height * m_height) / 12;
}