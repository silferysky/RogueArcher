#pragma once

class CircleCollider2DComponent // public BaseComponent
{
	float m_radius;
public:
	CircleCollider2DComponent() = default;
	~CircleCollider2DComponent() = default;

	float getRadius() const;
	void setRadius(float radius);
};