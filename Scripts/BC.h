#pragma once

class BC
{
	float m_radius;
public:
	BC(float radius = 0.0f) : m_radius{ radius }
	{}
	
	~BC() = default;

	float getRadius() const;
	void setRadius(const float radius);
};