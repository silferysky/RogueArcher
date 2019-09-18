#include "BC.h"

Vec2 BC::getCentre() const
{
	return m_centre;
}

float BC::getRadius() const
{
	return m_radius;
}

void BC::setCentre(const float x, const float y)
{
	m_centre.x = x;
	m_centre.y = y;
}

void BC::setRadius(const float r)
{
	m_radius = r;
}