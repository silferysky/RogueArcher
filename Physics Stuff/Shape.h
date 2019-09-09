#pragma once

enum ShapeType // List of possible convex hulls
{
	SHAPE_NONE,
	SHAPE_LINE = 2,
	SHAPE_TRI,
	SHAPE_RECT,
	SHAPE_CIRCLE = -1
};

// Class to define the shape's characteristics
class Shape
{ 
private:
	ShapeType m_numSides;
	float m_width;
	float m_height;
	float m_mass;
	float m_inertia; // Moment of inertia

public:
	Shape(ShapeType numSides = SHAPE_NONE, float w = 0.0f, float h = 0.0f, float m = 0.0f, float MoI = 0.0f)
		: m_numSides{ numSides }, m_width{ w }, m_mass{ m }, m_inertia{ MoI }
	{}

	virtual ~Shape();

	inline void initShape(float w, float h, float m);
	inline float calcInertia() const;

	friend class RigidbodyComponent;
	friend class PhysicsSystem;
};


class Line : public Shape
{
public:
	Line() : Shape(SHAPE_LINE)
	{}
	~Line()
	{}
};


class Triangle : public Shape
{
public:
	Triangle() : Shape(SHAPE_TRI)
	{}
	~Triangle()
	{}
};


class Rectangle : public Shape
{
public:
	Rectangle() : Shape(SHAPE_RECT)
	{}
	~Rectangle()
	{}
};


class Circle : public Shape
{
public:
	Circle() : Shape(SHAPE_CIRCLE)
	{}
	~Circle()
	{}
};

