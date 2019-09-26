#include "Circle.h"


Circle::Circle(float radius) : m_radius{ radius }
{}

Circle::~Circle()
{}

inline void Circle::setRadius(float rad)
{
	m_radius = rad;
}

inline float Circle::getRadius() const
{
	return m_radius;
}
