#pragma once

class CircleCollider2D // public BaseComponent
{
	float m_radius;
public:
	CircleCollider2D() = default;
	~CircleCollider2D() = default;

	float getRadius() const;
	void setRadius(float radius);
};