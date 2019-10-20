#pragma once

// Class to define the shape's characteristics
class Shape
{ 
private:
	float m_width;
	float m_height;
	float m_mass;
	float m_inertia; // Moment of inertia

public:
	Shape(float w = 0.0f, float h = 0.0f, float m = 0.0f, float MoI = 0.0f)
		: m_width{ w }, m_height{ h }, m_mass{ m }, m_inertia{ MoI }
	{}

	virtual ~Shape();

	inline void initShape(float w, float h, float m);
	inline float calcInertia() const;

	friend class RigidbodyComponent;
	friend class PhysicsSystem;
};