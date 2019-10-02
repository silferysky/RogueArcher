#include "TransformComponent.h"

void TransformComponent::setPosition(const Vec2& pos)
{
	m_position = pos;
}

void TransformComponent::offSetPosition(const Vec2& pos)
{
	m_position += pos;
}

void TransformComponent::setScale(const Vec2& scale)
{
	m_scale = scale;
}

void TransformComponent::offSetScale(const Vec2& scale)
{
	m_scale += scale;
}

void TransformComponent::setRotation(const float rot)
{
	m_rotation = rot;
}

void TransformComponent::offSetRotation(const float rot)
{
	m_rotation += rot;
}

Vec2 TransformComponent::getPosition() const
{
	return m_position;
}

Vec2 TransformComponent::getScale() const
{
	return m_scale;
}

float TransformComponent::getRotation() const
{
	return m_rotation;
}