#include "CircleCollider2DComponent.h"

float CircleCollider2DComponent::getRadius() const
{
	return m_radius;
}

void CircleCollider2DComponent::setRadius(float radius)
{
	m_radius = radius;
}