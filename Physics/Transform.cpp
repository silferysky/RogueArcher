#include "Transform.h"

void Transform::setPosition(const Vec2& pos)
{
	m_position = pos;
}

void Transform::offSetPosition(const Vec2& pos)
{
	m_position += pos;
}

void Transform::setScale(const Vec2& scale)
{
	m_scale = scale;
}

void Transform::offSetScale(const Vec2& scale)
{
	m_scale += scale;
}

void Transform::setRotation(const float rot)
{
	m_rotation = rot;
}

void Transform::offSetRotation(const float rot)
{
	m_rotation += rot;
}

Vec2 Transform::getPosition() const
{
	return m_position;
}

Vec2 Transform::getScale() const
{
	return m_scale;
}

float Transform::getRotation() const
{
	return m_rotation;
}